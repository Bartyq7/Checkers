#include "board.h"
#include "AI.h"

int main(int argc, char *argv[]) {
    Board board;
    board.display();

    Color currentPlayer = Color::BLACK;
    AI ai(Color::WHITE);

    while (true) {
        int from, to;
        char temp;
        if (currentPlayer == Color::BLACK) {
            std::cout << "Enter move : ";
            std::cin >> from >> temp >> to;
            if (temp == '-') {
                Coordinates move_cor = board.changeCoordinates(from, to);
                //Coordinates move_cor = board.changeCoordinates(from, to);


                if (board.moveChecker(move_cor, board)) {
                    board.display();
                    currentPlayer = Color::WHITE;
                } else {
                    std::cout << "Invalid move. Try again." << std::endl;
                }
            } else if (temp == 'x') {
                Coordinates capt_cor = board.changeCoordinates(from, to);
                //std::vector<Move> cur_seq;
                Move cur_seq = {capt_cor, true};
                //cur_seq.push_back({capt_cor, true});
                std::vector<Move> seq;

                board.available_jump_sequences(capt_cor.from, board, seq, cur_seq );
                for(int i=0;i<seq.size();i++){
                    //board.Capture(seq[i].mv_cor, board);
                    if(board.Capture(seq[i].mv_cor, board)){
                        board.display();
                        currentPlayer= Color::WHITE;
                    }else {
                        std::cout << "Invalid capture. Try again." << std::endl;
                    }
                }


                board.available_jump_sequences(capt_cor.from, board, seq, cur_seq );
//                if (board.Capture(capt_cor, board)) {
//                    board.display();
//                    currentPlayer = Color::WHITE;
//                } else {
//                    std::cout << "Invalid capture. Try again." << std::endl;
//                }
            } else {
                std::cout << "Bad notation. Try again." << std::endl;
            }
        } else {
            ai.makeMove(board, currentPlayer);
//            std::cout << "Enter move : ";
//            std::cin >> from >> temp >> to;
//            if (temp == '-') {
//                Coordinates move_cor = board.changeCoordinates(from, to);
//                //std::cout<<"fromx "<<move_cor.front().X<<"fromY "<<move_cor.front().Y<<"tox "<<move_cor.back().X<<"toY "<<move_cor.back().Y<<std::endl;
//
//                if (board.moveChecker(move_cor, board)) {
//                    board.display();
//                    currentPlayer = Color::BLACK;
//                } else {
//                    std::cout << "Invalid move. Try again." << std::endl;
//                }
//            } else if (temp == 'x') {
//                Coordinates capt_cor = board.changeCoordinates(from, to);
//                //std::vector<Move> cur_seq;
//                Move cur_seq = {capt_cor, true};
//                //cur_seq.push_back({capt_cor, true});
//                std::vector<Move> seq;
//                seq.push_back({capt_cor, true});
//                board.available_jump_sequences(capt_cor.from, board, seq, cur_seq );
//                for(int i=0;i<seq.size();i++){
//                    if(board.Capture(seq[i].mv_cor, board)){
//                        board.display();
//                        currentPlayer= Color::BLACK;
//                    }else {
//                        std::cout << "Invalid capture. Try again." << std::endl;
//                    }
//                }

//                for(int i=0;i<seq.size();i++){
//
//                    std::cout<<i<<"from "<<seq[i].mv_cor.from.X<<" "<<seq[i][j].mv_cor.from.Y<<"to "<<seq[i][j].mv_cor.to.X<<" "<<seq[i][j].mv_cor.to.Y<<std::endl;
//
//                }

//                if (board.Capture(capt_cor, board)) {
//                    board.display();
//                    currentPlayer = Color::BLACK;
//                } else {
//                    std::cout << "Invalid capture. Try again." << std::endl;
//                }
//            } else {
//                std::cout << "Bad notation. Try again." << std::endl;
//            }


        }
    }

    return 0;
}
