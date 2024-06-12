//
// Created by bkami on 28.05.2024.
//

#ifndef CHECKERS_AI_H
#define CHECKERS_AI_H
#include "board.h"

const int MIN = -10000;
const int MAX = 10000;
//const int DEPTH = 2;
class AI {
    Color AI_color;
    int DEPTH;
public:
    AI(Color ai_color, int DEPTH_ai);
    std::vector<Move> generateMoves(Board board_gen, Color color_to_gen);
    std::vector<Move> generateCaptures(Board board_gen, Color color_to_gen);
    int evaluate(Board board_ev) ;
    int minimax(Board board_minmax, int depth, bool maximizingPlayer, int alpha, int beta);
    Move findBestMove(Board board_fn);
    void makeMove(Board &board_makemove, std::string& output_string);
    std::vector<Move> findBestCapture(Board board_fncp);
    int changeCorFinal(int x, int y);
    void takeMove(Board &board, std::vector<Move> player_move);
    Move generateRandomMove(Board board_rm,  Color ai_col);
    std::vector<Move> findLongestCapture(const std::vector<std::vector<Move>> &vector_from);
};


#endif //CHECKERS_AI_H
