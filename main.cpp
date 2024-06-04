#include "board.h"
#include "AI.h"

int main() {
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
                if (board.moveChecker(move_cor)) {
                    board.display();
                    currentPlayer = Color::WHITE;
                } else {
                    std::cout << "Invalid move. Try again." << std::endl;
                }
            } else if (temp == 'x') {
                Coordinates capt_cor = board.changeCoordinates(from, to);
                if (board.Capture(capt_cor)) {
                    board.display();
                    currentPlayer = Color::WHITE;
                } else {
                    std::cout << "Invalid capture. Try again." << std::endl;
                }
            } else {
                std::cout << "Bad notation. Try again." << std::endl;
            }
        } else {
            std::cout << "AI is thinking..." << std::endl;
            Move bestMove=ai.findBestMove(board);
            if(!bestMove.isCapture){
                if (board.moveChecker(bestMove.mv_cor)) {
                    board.display();
                    currentPlayer = Color::BLACK;
                } else {
                    std::cout << "AI made an invalid move. Exiting." << std::endl;
                    break;
                }
            }else{
                if (board.Capture(bestMove.mv_cor)) {
                    board.display();
                    currentPlayer = Color::BLACK;
                } else {
                    std::cout << "AI made an invalid move. Exiting." << std::endl;
                    break;
                }
            }
        }
    }

    return 0;
}
