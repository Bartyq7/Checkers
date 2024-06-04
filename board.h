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
    Coordinates mv_cor;
    bool isCapture;
    bool isQueen;
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
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool moveChecker(Coordinates move_cor);
    Coordinates changeCoordinates(int from, int to);
    bool QueenCheck(Coordinates cor_que);
    bool canCapture(Coordinates cor) const;
    bool Capture(Coordinates capt_cor);
    void display() const;

};


#endif //CHECKERS_BOARD_H
