//
// Created by bkami on 25.05.2024.
//

#ifndef CHECKERS_BOARD_H
#define CHECKERS_BOARD_H

#include <vector>
#include <iostream>


const int BOARD_SIZE = 8;

enum class Color { NONE, BLACK, WHITE };

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
    Checker board[BOARD_SIZE][BOARD_SIZE];
public:
    Board();
    Color getFieldColor(int x, int y) const;
    bool getIfQueen(int x, int y) const;
    Checker getChecker(int x, int y)const;
    bool hasChecker(int x, int y) const;
    bool isSafe(int x, int y, Color color) const;
    void generateMovesForChecker(int x, int y, std::vector<Move>& moves);
    void generateCapturesForChecker(int x, int y, std::vector<Move>& moves);
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool moveChecker(Coordinates move_cor);
    Coordinates changeCoordinates(int from, int to);
    bool QueenCheck(Coordinates cor_que);
    bool canCapture(Coordinates capt_cor) const;
    bool Capture(Coordinates capt_cor);
    void generateMultipleCapture(int x, int y, std::vector<Move>& moves_mult);
    void display() const;
    void available_jump_sequences(Coordinates cor_cap, std::vector<Coordinates> current_sequence,
                                         Checker temp_board[BOARD_SIZE][BOARD_SIZE],
                                         std::vector<Coordinates> &sequences);
    std::vector<Move> available_cap_from(Position from, Checker temp_board[BOARD_SIZE][BOARD_SIZE]) const;
    void cap(Coordinates &capt_cor, Checker temp_board[BOARD_SIZE][BOARD_SIZE]);
};


#endif //CHECKERS_BOARD_H
