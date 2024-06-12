//
// Created by bkami on 28.05.2024.
//

#include "AI.h"

AI::AI(Color ai_color, int DEPTH_ai) {
    AI_color=ai_color;
    DEPTH = DEPTH_ai;
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
std::vector<Move> AI::generateCaptures(Board board_gen, Color color_to_gen)  {
    std::vector<Move> captures;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if ((i + j) % 2 != 0 && board_gen.getFieldColor(i,j) == color_to_gen) {
                //captures = board_gen.available_cap_from(i, j, board_gen);
                board_gen.generateCapturesForChecker(i,j, captures, board_gen);
                //std::cout<<"i "<<i<<"j "<<j<<std::endl;
            }
        }
    }
    return captures;
}
int AI::evaluate(Board board_ev){
    int score = 0;
    const int pawnValue = 5;
    const int queenValue = 50;
    const int zone1Bonus = 2;
    const int zone2Bonus = 1;
    const int level2Bonus = 1;
    const int level3Bonus = 3;
    const int level4Bonus = 20;
    const int captureBonus = 30;
    const int notSafePanish = -3;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board_ev.hasChecker(i, j)) {
                Checker checker = board_ev.getChecker(i, j);
                if (checker.isQueen) {
                    score += (checker.color == AI_color ? queenValue : 0);
                } else {
                    score += (checker.color == AI_color ? pawnValue : 0);
                }

                if ((i >= 2 && i <= 5) && (j >= 2 && j <= 5)) {
                    score += (checker.color == AI_color ? zone1Bonus : 0);
                }
                if ((i >= 1 && i <= 6) && (j >= 1 && j <= 6)) {
                    score += (checker.color == AI_color ? zone2Bonus : 0);
                }
                if(AI_color==Color::WHITE){
                    if (i >= 0 && i <= 1) {
                        score += (checker.color == AI_color ? level4Bonus : 0);
                    } else if(i>=2 && i<=3){
                        score += (checker.color == AI_color ? level3Bonus : 0);
                    } else if(i>=4 && i<=5){
                        score += (checker.color == AI_color ? level2Bonus : 0);
                    }
                }
                if(AI_color==Color::BLACK){
                    if (i >= 7 && i <= 8) {
                        score += (checker.color == AI_color ? level4Bonus : 0);
                    } else if(i>=5 && i<=6){
                        score += (checker.color == AI_color ? level3Bonus : 0);
                    } else if(i>=3 && i<=4){
                        score += (checker.color == AI_color ? level2Bonus : 0);
                    }
                }
//                if(checker.color==AI_color){
//                    if(!board_ev.isSafe(i, j, AI_color)){
//                        score += (checker.color == AI_color ? notSafePanish : 0);
//                    }
//                }
            }
        }
    }
    return score;
}
int AI::minimax(Board board_minmax, int depth, bool maximizingPlayer, int alpha, int beta) {
    if (depth == 0) {
        //newBoard.display();
        return evaluate(board_minmax);
    }
    //Color opponent_color = (AI_color == Color::BLACK ? Color::WHITE : Color::BLACK);
    std::vector<Move> moves = generateMoves(board_minmax, AI_color);
    //std::vector<Move> opponent_moves = generateMoves(board_minmax, opponent_color);

    if (maximizingPlayer) {
        int best = MIN;
        for (int i=0; i<moves.size();i++){
            board_minmax.moveChecker(moves[i].mv_cor, board_minmax);
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
            board_minmax.moveChecker(moves[i].mv_cor, board_minmax);
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

    for (int i=0;i<moves.size();i++) {
        board_fn.moveChecker(moves[i].mv_cor, board_fn);
        int moveValue = minimax(board_fn, DEPTH, true, MIN, MAX);
        //std::cout<<"move val "<<moveValue<<std::endl;

        if (moveValue >= bestValue) {
            bestMove = moves[i];
            bestValue = moveValue;
        }
    }
    return bestMove;
}
    //std::cout<<"bm cor to"<<bestMove.mv_cor.to.X<<bestMove.mv_cor.to.Y;


std::vector<Move> AI::findBestCapture(Board board_fncp) {
    std::vector<Move> capture_moves = generateCaptures(board_fncp, AI_color);
    //std::cout<<"size caputres: "<<captureMoves.size()<<std::endl;
    std::vector<std::vector<Move>> sequence;
    for(int index=0;index<capture_moves.size();index++){
        Board board_seq = board_fncp;
        board_seq.Capture(capture_moves[index].mv_cor,board_seq);
        std::vector<Move> current_seq;
        current_seq.push_back(capture_moves[index]);
        board_seq.capture_sequences(capture_moves[index].mv_cor.to, board_seq, sequence, current_seq);
    }
    std::vector<Move> bestCapture = findLongestCapture(sequence);
    return bestCapture;
}
void AI::makeMove(Board &board_makemove, std::string& output_string) {
    //std::cout << "AI is thinking..." << std::endl;
    std::vector<Move> sequence;
    bool no_multiple_capture = true;
    std::vector<Move> bestCapture = findBestCapture(board_makemove);

    if(!bestCapture.empty()) {
        for(int i=0;i<bestCapture.size();i++){
            if(board_makemove.Capture(bestCapture[i].mv_cor, board_makemove)){
                output_string = std::to_string(changeCorFinal(bestCapture[0].mv_cor.from.X, bestCapture[0].mv_cor.from.Y));
                for(int i=0;i<bestCapture.size();i++){
                    output_string += "x"+std::to_string(changeCorFinal(bestCapture[i].mv_cor.to.X, bestCapture[i].mv_cor.to.Y));
                }
            }
        }

    } else{
        Move bestMove=findBestMove(board_makemove);
        //std::cout<<"best from"<<bestMove.mv_cor.front().X<<" "<<bestMove.mv_cor.front().Y<<"best to"<<bestMove.mv_cor.back().X<<" "<<bestMove.mv_cor.back().Y<<"best cap "<<bestMove.isCapture<<std::endl;
        if( !bestMove.mv_cor.from.X==0 && !bestMove.mv_cor.from.Y==0 && !bestMove.mv_cor.to.X==0 && !bestMove.mv_cor.to.Y==0){
            if (board_makemove.moveChecker(bestMove.mv_cor, board_makemove)) {
                output_string = std::to_string(changeCorFinal(bestMove.mv_cor.from.X, bestMove.mv_cor.from.Y))+"-"+
                        std::to_string(changeCorFinal(bestMove.mv_cor.to.X, bestMove.mv_cor.to.Y));
                //turn = (turn == Color::BLACK ? Color::WHITE:Color::BLACK);
            } else {
                //std::cout << "AI made an invalid move. Exiting." << std::endl;
                return;
            }
        }else{
            //std::cout<<"cosik"<<std::endl;
            bestMove = generateRandomMove(board_makemove, AI_color);
            // std::cout<<"best from"<<bestMove.mv_cor.from.X<<" "<<bestMove.mv_cor.from.Y<<"best to"<<bestMove.mv_cor.to.X<<" "
            // <<bestMove.mv_cor.to.Y<<"best cap "<<bestMove.isCapture<<std::endl;
            if (board_makemove.moveChecker(bestMove.mv_cor, board_makemove)) {
                output_string = std::to_string(changeCorFinal(bestMove.mv_cor.from.X, bestMove.mv_cor.from.Y))+"-"+
                        std::to_string(changeCorFinal(bestMove.mv_cor.to.X, bestMove.mv_cor.to.Y));
                //turn = (turn == Color::BLACK ? Color::WHITE:Color::BLACK);
            } else {
                //std::cout << "AI made an invalid move. Exiting." << std::endl;
                return;
            }
        }
    }
}
int AI::changeCorFinal(int x, int y){
    int num=x*4+y/2;
    return num+1;
}
void AI::takeMove(Board &board, std::vector<Move> player_move) {


    for (int i = 0; i < player_move.size(); i++) {
//        std::cout << "Move 1: From (" << player_move[i].mv_cor.from.X << "," << player_move[i].mv_cor.from.Y
//                  << ") to (" << player_move[i].mv_cor.to.X
//                  << "," << player_move[i].mv_cor.to.Y << ")" << "cap " << player_move[i].isCapture
//                  << " size :" << player_move.size() << std::endl;
        if (!player_move[i].isCapture) {
            board.moveChecker(player_move[i].mv_cor, board);
        } else if (player_move[i].isCapture) {
            board.Capture(player_move[i].mv_cor, board);
        } else {
            std::cerr << "Invalid move" << std::endl;
            return;
        }
    }
}
Move AI::generateRandomMove(Board board_rm, Color ai_col){
    Move random_move;
    for(int i=0; i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            if(board_rm.getFieldColor(i, j)==ai_col){
                for(int z=-1;z<=1;z++){
                    for(int y=-1;y<=1;y++){
                        random_move.mv_cor.from.X=i;
                        random_move.mv_cor.from.Y=j;
                        random_move.mv_cor.to.X=z+i;
                        random_move.mv_cor.to.Y=y+j;
                        random_move.isCapture=false;
                        if(board_rm.isMoveValid(random_move.mv_cor.from.X, random_move.mv_cor.from.Y, random_move.mv_cor.to.X, random_move.mv_cor.to.Y)){
                            return random_move;
                        }
                    }
                }
            }
        }
    }
    return random_move;
}
std::vector<Move> AI::findLongestCapture(const std::vector<std::vector<Move>> &vector_from){
    std::vector<Move> longest;
    int max_size=0;
    for(int i=0; i<vector_from.size();i++){
        if(vector_from[i].size()>max_size){
            longest=vector_from[i];
            max_size=vector_from[i].size();
        }
    }
    return longest;
}