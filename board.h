//
// Created by bkami on 25.05.2024.
//

#ifndef CHECKERS_BOARD_H
#define CHECKERS_BOARD_H

#include <vector>
#include <iostream>



const int BOARD_SIZE = 8;

enum class Color { NONE, BLACK, WHITE, VOID };

struct Checker {
    Color color;
    bool isQueen;

    Checker(Color c = Color::NONE, bool Queen = false) : color(c), isQueen(Queen) {}
};
struct Position{
    int X;
    int Y;
};
struct Coordinates{
    Position from;
    Position to;
};
struct Move {
    // w moim kodzie poczatek(begin) vektora to zawsze pozycja z ktorej sie przemieszczamy(from),
    // a koniec(end) vektora to pozycja do ktorej docelowo sie przemieszczamy(to).
    Coordinates mv_cor;
    bool isCapture;
    //bool isQueen;
};


////////////////////////////////////////////////
class Board {
    std::vector<std::vector<Checker>> board;
public:
    Board();
    Color getFieldColor(int x, int y) const;
    bool getIfQueen(int x, int y) const;
    Checker getChecker(int x, int y)const;
    std::vector<std::vector<Checker>> getCurrentBoard();
    bool hasChecker(int x, int y) const;
    bool isSafe(int x, int y, Color color) const;
    void generateMovesForChecker(int x, int y, std::vector<Move>& moves);
    void generateCapturesForChecker(int x, int y, std::vector<Move> &captures, Board board_gncp);
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool moveChecker(Coordinates move_cor, Board &board_move);
    bool QueenCheck(Coordinates cor_que, Board &board_queen);
    bool canCapture(Coordinates capt_cor, Board board_cancap) const;
    bool Capture(Coordinates capt_cor, Board &board_cap);
    void display() const;
    void available_capture_sequences(Position from, Board temp_board, std::vector<Move> &sequence, Move current_sequence);
    std::vector<Move> available_captures_from(int x, int y, Board &board_av) const;
    bool checkEndGame();

    //std::vector<std::vector<Checker>> copyBoard(Checker board_from[BOARD_SIZE][BOARD_SIZE]);
};


#endif //CHECKERS_BOARD_H
