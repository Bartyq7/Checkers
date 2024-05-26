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

class Board {
    Checker board[BOARD_SIZE][BOARD_SIZE];
public:
    Board();
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool moveChecker(int from, int to);
    Coordinates changeCoordinates(int from, int to);
    bool QueenCheck(Coordinates cor_que);
    bool canCapture(Coordinates cor) const;
    bool Capture(int from, int to);
    void display() const;

};


#endif //CHECKERS_BOARD_H
