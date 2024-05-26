#include "board.h"

int main() {
    Board board;
    board.display();

    int from, to;
    char temp;
    while (true) {
        std::cout << "Enter move : ";
        std::cin >> from >> temp >> to;
        if(temp == '-'){
            if (board.moveChecker(from, to)) {
                board.display();
            } else {
                std::cout << "Invalid move. Try again." << std::endl;
            }
        }
        else if(temp == 'x'){
            if (board.Capture(from, to)) {
                board.display();
            }else {
                std::cout << "Invalid capture. Try again." << std::endl;
            }
        } else{std::cout<<"Bad notation. Try again."<<std::endl;}
    }

    return 0;
}