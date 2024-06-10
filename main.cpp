#include "board.h"
#include "AI.h"
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>

#define BUFSPACE 1024


bool is_move(const std::string& move) {
    return move.find('x') == std::string::npos;
}

Coordinates changeCoordinates(int from, int to) {
    Coordinates cor;
    int ind;
    ind=from-1;
    cor.from.X= ind/4;
    if(cor.from.X%2==0){
        cor.from.Y=(ind%4)*2 +1;
    } else{
        cor.from.Y = (ind % 4)*2;
    }
    ind=to-1;
    cor.to.X= ind/4;
    if(cor.to.X%2==0){
        cor.to.Y=(ind%4)*2 +1;
    } else{
        cor.to.Y = (ind % 4)*2;
    }
    return cor;
}

std::vector<int> extract_positions(const std::string& move) {
    std::vector<int> positions;
    std::string number;

    for (char ch : move) {
        if (std::isdigit(ch)) {
            number += ch;
        } else {
            if (!number.empty()) {
                positions.push_back(std::stoi(number));
                number.clear();
            }
        }
    }

    if (!number.empty()) {
        positions.push_back(std::stoi(number));
    }

    return positions;
}

std::vector<Move> parse_move(const std::string& move) {
    std::vector<Move> coords;
    auto positions = extract_positions(move);

    if (is_move(move)) {
        if (positions.size() <= 2) {
            coords.push_back({changeCoordinates(positions[0], positions[1]), false});
        } else{
            std::cout<<"Invalid notation"<<std::endl;
        }
    } else {
        for(int i=0;i<positions.size()-1;i++){
        coords.push_back({changeCoordinates(positions[i], positions[i+1]), true});
        }
    }

    return coords;
}

int main(int argc, char *argv[]) {
    char buf[BUFSPACE];
    int serv_sock;
    //struct sockaddr_in serv_addr;
    //struct hostent *serv_hostent;

    std::string interface=argv[1];

    std::string turn = argv[2];

    int depth = atoi(argv[3]);

    Board board;
    Color oponent, ai_color;

    int random_seed;
    std::string ip_address;
    int ip_port;

    if(turn == "WHITE"){
        ai_color = Color::WHITE;
        oponent = (turn=="WHITE" ? Color::BLACK: Color::WHITE);
    }
    if(turn == "BLACK"){
        ai_color = Color::BLACK;
        oponent = (turn=="BLACK" ? Color::WHITE: Color::BLACK);
    }


    AI ai(ai_color, depth);
    Color currentPlayer = oponent;


    if(argc == 4){
        srand(time(NULL));
    }
    else if(argc == 5){
        random_seed = atoi(argv[4]);
        srand(random_seed);
        //std::cout<<"seed"<<random_seed<<std::endl;
    }
    else if(argc == 6){
        srand(time(NULL));
        ip_address = argv[4];
        ip_port = atoi(argv[5]);
        //std::cout<<"address"<<ip_address<<" port"<<ip_port<<std::endl;
    }
    else if(argc ==7){
        random_seed = atoi(argv[4]);
        //std::cout<<"seed"<<random_seed<<std::endl;
        srand(random_seed);
        ip_address = argv[5];
        ip_port = atoi(argv[6]);
        //std::cout<<"address"<<ip_address<<" port"<<ip_port<<std::endl;
    } else{
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(interface == "GUI") {
        board.display();
        if (ai_color == Color::WHITE) {///////////////////////////////////////////////////////
            while (!board.checkEndGame()) {
                if (currentPlayer == Color::BLACK) {
                    if (oponent == Color::WHITE) {
                        std::cout << "Enter move for White: " << std::endl;
                    } else if (oponent == Color::BLACK) {
                        std::cout << "Enter move for Black: " << std::endl;
                    } else {
                        std::cout << "You didn't pick any color" << std::endl;
                        break;
                    }
                    std::string buf_string;
                    std::cin >> buf_string;
                    std::vector<Move> player_move = parse_move(buf_string);

                    for (int i = 0; i < player_move.size(); i++) {
                        std::cout << "Move 1: From (" << player_move[i].mv_cor.from.X << "," << player_move[i].mv_cor.from.Y
                                  << ") to (" << player_move[i].mv_cor.to.X
                                  << "," << player_move[i].mv_cor.to.Y << ")" << "cap " << player_move[i].isCapture
                                  << " size :" << player_move.size() << std::endl;
                        if (!player_move[i].isCapture) {
                            board.moveChecker(player_move[i].mv_cor, board);
                            currentPlayer = Color::WHITE;
                        } else if (player_move[i].isCapture) {
                            board.Capture(player_move[i].mv_cor, board);
                            currentPlayer = Color::WHITE;
                        } else {
                            std::cout << "Invalid move" << std::endl;
                            break;
                        }
                    }
                    board.display();
                } else {
                    std::string output_string;
                    ai.makeMove(board, output_string);
                    std::cout << "AI moved: " << output_string << std::endl;
                    currentPlayer = Color::BLACK;
                    board.display();
                }
            }
        } else if(ai_color==Color::BLACK){//////////////////////////////////
            while (!board.checkEndGame()) {
                if (currentPlayer == Color::BLACK) {
                    std::string output_string;
                    ai.makeMove(board, output_string);
                    std::cout << "AI moved: " << output_string << std::endl;
                    currentPlayer = Color::WHITE;
                    board.display();
                } else {
                    if (oponent == Color::WHITE) {
                        std::cout << "Enter move for White: " << std::endl;
                    } else if (oponent == Color::BLACK) {
                        std::cout << "Enter move for Black: " << std::endl;
                    } else {
                        std::cout << "You didn't pick any color" << std::endl;
                        break;
                    }
                    std::string buf_string;
                    std::cin >> buf_string;
                    std::vector<Move> player_move = parse_move(buf_string);

                    for (int i = 0; i < player_move.size(); i++) {
                        std::cout << "Move 1: From (" << player_move[i].mv_cor.from.X << "," << player_move[i].mv_cor.from.Y
                                  << ") to (" << player_move[i].mv_cor.to.X
                                  << "," << player_move[i].mv_cor.to.Y << ")" << "cap " << player_move[i].isCapture
                                  << " size :" << player_move.size() << std::endl;
                        if (!player_move[i].isCapture) {
                            board.moveChecker(player_move[i].mv_cor, board);
                        } else if (player_move[i].isCapture) {
                            board.Capture(player_move[i].mv_cor, board);
                        } else {
                            std::cout << "Invalid move" << std::endl;
                            break;
                        }
                    }
                    currentPlayer = Color::BLACK;
                    board.display();
                }
            }
        }
    }
    ////////////////////////////////////////////////////
    else if(interface=="NET"){
//
//        serv_sock = socket(AF_INET, SOCK_STREAM, 0);
//        if(serv_sock < 0)
//        {
//            perror("socket");
//            exit(errno);
//        }
//        serv_hostent = gethostbyname(ip_address);
//        if(serv_hostent == 0)
//        {
//            std::cerr<<"[Error: "<<argv[0]<<"] Nieznany adres IP: "<<ip_address<<std::endl;
//            exit(-1);
//        }
//        serv_addr.sin_family = AF_INET;
//        memcpy(&serv_addr.sin_addr, serv_hostent->h_addr, serv_hostent->h_length);
//        serv_addr.sin_port = htons(ip_port);
//
//        std::cout<<"Laczymy sie z serwerem"<<std::endl;
//        if(connect(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
//        {
//            perror("connect");
//            exit(-1);
//        }
//
//        std::cout<<"Polaczenie nawiazane, zaczynamy gre"<<std::endl;
//
//        int n;
//        bool myturn;
//
//
//
//        if(ai_color==Color::Black){
//            myturn=true;
//        }  //Czarny zaczyna
//        else{
//            myturn=false;
//        }              //Biały czeka
//        while(true)
//        {
//            if(myturn)
//            {
//                std::string output_string;
//                ai.makeMove(board, output_string);
//                std::cout<<"Wysylam do serwera moj ruch: "<<output_string<<std::endl;
//                for(int i=0; i<output_string.size(); i++){
//                    buf[i]=output_string[i];
//                }
//                buf[output_string.size()]=0;
//                if(write(serv_sock, buf, output_string.size()) < 0)
//                {
//                    perror("write");
//                    exit(errno);
//                }
//                myturn=false;
//            }
//            std::cout<<"Czekam na ruch przeciwnika..."<<std::endl;
//
//            n=read(serv_sock, buf, sizeof buf);
//
//            if(n<0)
//            {
//                perror("read");
//                exit(errno);
//            }
//            if(n==0)
//            { /* pusty komunikat = zamkniete polaczenie */
//                std::cout<<"Broker zamknal polaczenie"<<std::endl;
//                exit(0);
//            }
//            buf[n]=0;
//
//            std::cout<<"Otrzymalem ruch przeciwnika: "<<buf<<std::endl;
//
//            std::string input_string=buf;
//
//            std::vector<Move> player_move = parse_move(input_string);
//            ai.takeMove(board, player_move);
//            myturn=true;
//        }
//

    } else{
        return 0;
    }
    return 0;
}
