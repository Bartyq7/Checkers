//
// Created by bkami on 28.05.2024.
//

#ifndef CHECKERS_AI_H
#define CHECKERS_AI_H
#include "board.h"

const int MIN = -10000;
const int MAX = 10000;
const int DEPTH = 2;
class AI {
    Color AI_color;
    //Checker board[BOARD_SIZE][BOARD_SIZE];
    //Board *current_board;
public:
    AI(Color ai_color);
    //void get_current_board();
    std::vector<Move> generateMoves(Board board_gen, Color color_to_gen);
    std::vector<Move> generateCaptures(Board board_gen, Color color_to_gen);
    int evaluate(Board board_ev) ;
    int minimax(Board board_minmax, int depth, bool maximizingPlayer, int alpha, int beta);
    Move findBestMove(Board board_fn);
    //int findMaxValueIndex(const std::vector<Move>& moves);
    void makeMove(Board &board_makemove, Color &turn);
    std::vector<Move> findBestCapture(Board board_fncp);
};


#endif //CHECKERS_AI_H
