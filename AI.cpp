//
// Created by bkami on 28.05.2024.
//

#include "AI.h"

AI::AI(Color ai_color) {
    AI_color=ai_color;
}

std::vector<Move> AI::generateMoves(Board board_gen, Color color_to_gen) {
    std::vector<Move> moves;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 != 0 && board_gen.getFieldColor(i,j) == color_to_gen) {
                board_gen.generateMovesForChecker(i, j, moves);
                //std::cout<<"i "<<i<<"j "<<j<<std::endl;
            }
        }
    }
    return moves;
}
/*
void AI::generateMovesForChecker(int x, int y, std::vector<Move>& moves, Board board_gen) const {
    static const Position directions[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};

    for (int i=0; i<4; i++) {
        int newX = x + directions[i].X;
        int newY = y + directions[i].Y;
        Coordinates cor = {{x, y}, {newX, newY}};
        if (board_gen.isMoveValid(cor.from.X, cor.from.Y, cor.to.X, cor.to.Y)) {
            //std::cout<<"cor from "<<cor.from.X<<cor.from.Y<<" cor to "<<cor.to.X<<cor.to.Y<<std::endl;
            moves.push_back({cor, false, board_gen.QueenCheck(cor)});
        }
    }

    for (int i=0; i<4; i++) {
        int newX = x + captureDirections[i].X;
        int newY = y + captureDirections[i].Y;
        Coordinates cor = {{x, y}, {newX, newY}};
        if (board_gen.canCapture(cor)) {
            moves.push_back({cor, true,board_gen.QueenCheck(cor)});
        }
    }
}*/
int AI::evaluate(Board board_ev){
    int score = 0;
    const int pawnValue = 1;
    const int queenValue = 5;
    const int centralControlBonus = 2;
    const int edgePenalty = -1;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board_ev.hasChecker(i, j)) {
                Checker checker = board_ev.getChecker(i, j);
                if (checker.isQueen) {
                    score += (checker.color == AI_color ? queenValue : 0);
                } else {
                    score += (checker.color == AI_color ? pawnValue : 0);
                }

                if ((i >= 3 && i <= 4) && (j >= 2 && j <= 4)) {
                    score += (checker.color == AI_color ? centralControlBonus : 0);
                }

                // Dodaj punkty za bezpieczeństwo (czy pionek jest zagrożony przejęciem)
//                if (!board_ev.isSafe(i, j, checker.color)) {
//                    score += (checker.color == AI_color ? edgePenalty : 0);
//                }
            }
        }
    }
    return score;
}
int AI::minimax(Board board_minmax, int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0) {
        //std::cout<<"Chuj"<<std::endl;
        //newBoard.display();
        return evaluate(board_minmax);
    }
    //Color opponent_color = (AI_color == Color::BLACK ? Color::WHITE : Color::BLACK);
    std::vector<Move> moves = generateMoves(board_minmax, AI_color);
    //std::vector<Move> opponent_moves = generateMoves(board_minmax, opponent_color);

    if (maximizingPlayer) {
        int best = MIN;
        for (int i=0; i<moves.size();i++){
            board_minmax.moveChecker(moves[i].mv_cor);
            int val = minimax(board_minmax, depth - 1, false, alpha, beta);
            //std::cout<<"val "<<i<<" "<<val<<std::endl;
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (beta <= alpha)
                break;
        }
        return best;
    } else {
        int best = MAX;

        for (int i=0; i<moves.size(); i++) {
            board_minmax.moveChecker(moves[i].mv_cor);
            int val = minimax(board_minmax, depth - 1, true, alpha, beta);
            //std::cout<<"val "<<i<<" "<<val<<std::endl;
            //std::cout<<"best "<<i<<" "<<best<<std::endl;
            best = std::min(best, val);
            //std::cout<<"best af "<<i<<" "<<best<<std::endl;
            beta = std::min(beta, best);

            if (beta <= alpha)
                break;
        }
        return best;
    }
}
Move AI::findBestMove(Board board_fn) {
    std::vector<Move> moves = generateMoves(board_fn, AI_color);
    int bestValue = MIN;
    Move bestMove;
    std::vector<Move> captureMoves;
    for (int i=0;i<moves.size();i++) {
        if(moves[i].isCapture){
            captureMoves.push_back(moves[i]);
        }
    }
    if(captureMoves.empty()){
        for (int i=0;i<moves.size();i++) {
            board_fn.moveChecker(moves[i].mv_cor); // Assuming moveChecker applies the move
            //std::cout<<std::endl;
            //newBoard.display();
            int moveValue = minimax(board_fn, DEPTH, true, MIN, MAX);
            std::cout<<"move val "<<moveValue<<std::endl;

            if (moveValue >= bestValue) {
                bestMove = moves[i];
                bestValue = moveValue;
            }
        }
    } else{
        for (int i=0;i<captureMoves.size();i++) {
            board_fn.moveChecker(captureMoves[i].mv_cor); // Assuming moveChecker applies the move
            //std::cout<<std::endl;
            //newBoard.display();
            int capValue = minimax(board_fn, DEPTH, true, MIN, MAX);
            std::cout<<"cap val "<<capValue<<std::endl;

            if (capValue >= bestValue) {
                bestMove = captureMoves[i];
                bestValue = capValue;
            }
        }
    }
    //std::cout<<"bm cor to"<<bestMove.mv_cor.to.X<<bestMove.mv_cor.to.Y;
    return bestMove;
}
/*
int AI::findMaxValueIndex(const std::vector<Move>& moves) {
    int maxValue = MIN;
    int maxIndex = -1;

    for (int i = 0; i < moves.size(); ++i) {
        if (moves[i].score > maxValue) {
            maxValue = moves[i].score;
            maxIndex = i;
        }
    }
    return maxIndex;
}
*/