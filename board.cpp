//
// Created by bkami on 25.05.2024.
//

#include "board.h"


Board::Board(): board(BOARD_SIZE, std::vector<Checker>(BOARD_SIZE)) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if ((i + j) % 2 != 0) {
                if (i < 3) {
                    board[i][j] = Checker(Color::BLACK);
                }
                else if (i > 4) {
                    board[i][j] = Checker(Color::WHITE);
                }
            }
        }
    }
}
Color Board::getFieldColor(int x, int y) const{
    return board[x][y].color;
}
bool Board::getIfQueen(int x, int y) const{
    return board[x][y].isQueen;
}
Checker Board::getChecker(int x, int y) const {
    return board[x][y];
}
std::vector<std::vector<Checker>> Board::getCurrentBoard(){
    return board;
}
bool Board::hasChecker(int x, int y) const{
    if(board[x][y].color!=Color::NONE){
        return true;
    }
    return false;
}

//bool Board::isSafe(int x, int y, Color color) const {
//    static const int dx[] = { -1, -1, 1, 1 };
//    static const int dy[] = { -1, 1, -1, 1 };
//    static const int captureDx[] = { -2, -2, 2, 2 };
//    static const int captureDy[] = { -2, 2, -2, 2 };
//
//    // Sprawdź, czy na polu (x, y) znajduje się pionek o podanym kolorze
//    if (!hasChecker(x, y) || getChecker(x, y).color != color) {
//        return true; // Jeśli nie ma pionka lub jest innego koloru, jest bezpieczny (nie ma pionka do przejęcia)
//    }
//
//    // Sprawdź, czy przeciwnik może zbić pionek
//    Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
//
//    for (int i = 0; i < 4; ++i) {
//        int overX = x + dx[i];
//        int overY = y + dy[i];
//        int toX = x + captureDx[i];
//        int toY = y + captureDy[i];
//        Coordinates cor_sf = {{x,y},{toX, toY}};
//        // Sprawdź, czy przeciwnik może przeskoczyć (x, y)
//        if (isMoveValid(overX, overY, toX, toY) && hasChecker(overX, overY)) {
//            Checker overChecker = getChecker(overX, overY);
//
//            if (overChecker.color == opponentColor && canCapture(cor_sf)) {
//                return false; // Pionek nie jest bezpieczny, przeciwnik może go zbić
//            }
//        }
//    }
//
//    return true; // Pionek jest bezpieczny
//}
bool Board::isSafe(int x, int y, Color color) const {
    int behind_X[2]={-1,1};
    int behind_Y;
    int leftBehindX, leftBehindY;
    int rightBehindX, rightBehindY;
    if(color==Color::BLACK){
        behind_Y = -1;
    }
    if(color == Color::WHITE){
        behind_Y = 1;
    }
    leftBehindX = x+behind_X[0];
    rightBehindX = x+behind_X[1];
    leftBehindY = y+behind_Y;
    rightBehindY = y+behind_Y;

    if(board[leftBehindX][leftBehindY].color==color && leftBehindX>=0 &leftBehindX<BOARD_SIZE && leftBehindY>=0 &&leftBehindY<BOARD_SIZE){
        return true;
    } else if(board[rightBehindX][rightBehindY].color == color && rightBehindX>=0 &rightBehindX<BOARD_SIZE && rightBehindY>=0 && rightBehindY<BOARD_SIZE){
        return true;
    }
    return false;
}
void Board::generateMovesForChecker(int x, int y, std::vector<Move>& moves) {
    static const Position directions[4] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};

    for (int i=0; i<4; i++) {
        int newX = x + directions[i].X;
        int newY = y + directions[i].Y;
        //Coordinates cor = {{x, y}, {newX, newY}};
        Coordinates cor = {{x,y},{newX,newY}};
        if (isMoveValid(cor.from.X, cor.from.Y, cor.to.X, cor.to.Y)) {
            //std::cout<<"cor from "<<cor.from.X<<cor.from.Y<<" cor to "<<cor.to.X<<cor.to.Y<<std::endl;
            moves.push_back({cor, false});
        }
    }
//    for (int i=0; i<4; i++) {
//        int newX = x + captureDirections[i].X;
//        int newY = y + captureDirections[i].Y;
//        Coordinates cor = {{x, y}, {newX, newY}};
//        if (canCapture(cor)) {
//            moves.push_back({cor, true});
//        }
//    }

    //generateMultipleCapture(x,y,moves);
}
void Board::generateCapturesForChecker(int x, int y,std::vector<Move> &captures, Board board_gncp){
    //std::vector<Move> captures;
    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
    for (int i=0; i<4; i++) {
        int newX = x + captureDirections[i].X;
        int newY = y + captureDirections[i].Y;
        Coordinates cor = {{x, y}, {newX, newY}};
        if (canCapture(cor, board_gncp)) {
            captures.push_back({cor, true});
        }
    }
}
bool Board::isMoveValid(int fromX, int fromY, int toX, int toY) const {
    if (toX < 0 || toX >= BOARD_SIZE || toY < 0 || toY >= BOARD_SIZE) {
        return false;
    }
    if((fromX+fromY)%2 == 0){
        return false;
    }
    if((toX+toY)%2 == 0){
        return false;
    }
    if (board[toX][toY].color != Color::NONE) {
        return false;
    }
    if(board[fromX][fromY].color==Color::WHITE && !board[fromX][fromY].isQueen && toX > fromX){
        return false;
    }
    if(board[fromX][fromY].color==Color::BLACK && !board[fromX][fromY].isQueen && toX < fromX){
        return false;
    }


    int dx = toX - fromX;
    int dy = toY - fromY;

    if (abs(dx) != 1 || abs(dy) != 1) {
        return false;
    }

    return true;
}


bool Board::moveChecker(Coordinates move_cor, Board &board_move) {
    //Coordinates move_cor = changeCoordinates(from, to);
    if (!isMoveValid(move_cor.from.X, move_cor.from.Y, move_cor.to.X, move_cor.to.Y)) {
        return false;
    }



    board_move.board[move_cor.to.X][move_cor.to.Y] = board_move.board[move_cor.from.X][move_cor.from.Y];
    board_move.board[move_cor.from.X][move_cor.from.Y] = Checker();
    QueenCheck(move_cor, board_move);
    return true;
}
bool Board::QueenCheck(Coordinates cor_que, Board &board_queen) {
    //////////////////zamiana w krolowa///////////////////////////
    if(board_queen.board[cor_que.to.X][cor_que.to.Y].color == Color::BLACK && cor_que.to.X == 0){
        board_queen.board[cor_que.to.X][cor_que.to.Y].isQueen = true;
        std::cout<<"to       "<<board_queen.board[cor_que.to.X][cor_que.to.Y].isQueen<<std::endl;
        return true;
    }
    if(board_queen.board[cor_que.to.X][cor_que.to.Y].color == Color::WHITE && cor_que.to.X == BOARD_SIZE-1){
        board_queen.board[cor_que.to.X][cor_que.to.Y].isQueen = true;
        return true;
    }
    return false;
    //////////////////////////////////////////////////////////////
}
bool Board::canCapture(Coordinates cor, Board board_cancap) const {
    if (cor.from.X < 0 || cor.from.X >= BOARD_SIZE || cor.from.Y < 0 || cor.from.Y >= BOARD_SIZE) {
        return false;
    }
    if (cor.to.X < 0 || cor.to.X >= BOARD_SIZE || cor.to.Y < 0 || cor.to.Y >= BOARD_SIZE) {
        return false;
    }
    if((cor.from.X+cor.from.Y)%2 == 0){
        return false;
    }
    if((cor.to.X+cor.to.Y)%2 == 0){
        return false;
    }
    if (board_cancap.getFieldColor(cor.to.X, cor.to.Y) != Color::NONE) {
        return false;
    }
    if (board_cancap.getFieldColor(cor.from.X, cor.from.Y) == Color::NONE) {
        return false;
    }

    int midX = (cor.from.X + cor.to.X) / 2;
    int midY = (cor.from.Y + cor.to.Y) / 2;

    if (board_cancap.getFieldColor(midX, midY) == Color::NONE || board_cancap.getFieldColor(midX, midY) == board_cancap.getFieldColor(cor.from.X, cor.from.Y)) {
        return false;
    }
    if(board_cancap.getFieldColor(cor.from.X, cor.from.Y)==Color::WHITE && !board_cancap.getIfQueen(cor.from.X, cor.from.Y) && cor.to.X > cor.from.X){
        return false;
    }
    if(board_cancap.getFieldColor(cor.from.X, cor.from.Y)==Color::BLACK && !board_cancap.getIfQueen(cor.from.X, cor.from.Y) && cor.to.X < cor.from.X){
        return false;
    }


    return true;
}
bool Board::Capture(Coordinates capt_cor, Board &board_cap) {
    Position captured;
    //Coordinates capt_cor = changeCoordinates(from, to);
    if(!canCapture(capt_cor, board_cap)){
        return false;
    }
    int midX = (capt_cor.from.X + capt_cor.to.X)/2;
    int midY = (capt_cor.from.Y + capt_cor.to.Y)/2;



    board_cap.board[capt_cor.to.X][capt_cor.to.Y] = board_cap.board[capt_cor.from.X][capt_cor.from.Y];
    board_cap.board[capt_cor.from.X][capt_cor.from.Y] = Checker();
    board_cap.board[midX][midY] = Checker();
    QueenCheck(capt_cor, board_cap);

    return true;
}
//void Board::generateCapturesForChecker(int x, int y, std::vector<Move>& moves) {
//    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
//    for (int i=0; i<4; i++) {
//        int newX = x + captureDirections[i].X;
//        int newY = y + captureDirections[i].Y;
//        std::vector<Position> cor = {{x, y}, {newX, newY}};
//        if (canCapture(cor)) {
//            moves.push_back({cor, true});
//        }
//    }
//}


void Board::display() const {
    int field=1;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j].color == Color::NONE) {
                if((i+j)%2 == 0){
                    std::cout<<"  ";
                }
                else {
                    std::cout << field << "* ";
                    field++;
                }
            } else if (board[i][j].color == Color::WHITE) {
            std::cout << "W" << field << " ";
            field++;
        } else if (board[i][j].color == Color::BLACK) {
            std::cout << "B" << field << " "; // Black color
            field++;
        } else if (board[i][j].isQueen && board[i][j].color == Color::WHITE) {
            std::cout << "QW" << field << " "; // White Queen color
            field++;
        } else if (board[i][j].isQueen && board[i][j].color == Color::BLACK) {
            std::cout << "QB" << field << " "; // Black Queen color
            field++;
            } else{std::cout<<"cos nie tak\n";}
        }
        std::cout << std::endl;
    }
}

bool Board::capture_sequences(Position &from, Board temp_board, std::vector<Move> &sequence, Move current_sequence, Color ai_color) {
        std::vector<Move> captureMoves = available_captures_from(from.X, from.Y, temp_board);
    bool multipleCapturesFound = false;

    for (Move move : captureMoves) {
        if (move.isCapture) {
            temp_board.Capture(move.mv_cor, temp_board);
            sequence.push_back(move);

            if (capture_sequences(move.mv_cor.to, temp_board, sequence, move, ai_color)) {
                multipleCapturesFound = true;
            }

            temp_board.undoCapture(temp_board,move, ai_color);
            sequence.pop_back();
        }
    }

    if (!captureMoves.empty() && sequence.size() > 1) {
        multipleCapturesFound = true;
    }

    return multipleCapturesFound;
}

std::vector<Move> Board::available_captures_from(int x, int y, Board &board_av) const {
    std::vector<Move> captures;
    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
    for (int i=0; i<4; i++) {
        int newX = x + captureDirections[i].X;
        int newY = y + captureDirections[i].Y;
        Coordinates cor = {{x, y}, {newX, newY}};
            if (canCapture(cor, board_av)) {
                captures.push_back({cor, true});
            }
    }
    return captures;
}
bool Board::checkEndGame(){
    bool blackWins = false;
    bool whiteWins = false;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if ((i + j) % 2 != 0) {
               if(board[i][j].color==Color::WHITE){
                   whiteWins= true;
               }
                else if(board[i][j].color==Color::BLACK){
                    blackWins= true;
                }
            }
        }
    }
    if(whiteWins == true && blackWins == false){
        std::cout<<"White wins";
        return true;
    }
    if(blackWins == true && whiteWins == false){
        std::cout<<"Black wins";
        return true;
    }
    return false;
}
void Board::undoCapture(Board &temp_board, Move capture, Color ai_color){
    int midX = (capture.mv_cor.from.X + capture.mv_cor.to.X)/2;
    int midY = (capture.mv_cor.from.Y + capture.mv_cor.to.Y)/2;
    Color oponent_color=(ai_color==Color::BLACK?Color::WHITE:Color::BLACK);


    temp_board.board[capture.mv_cor.from.X][capture.mv_cor.from.Y] = temp_board.board[capture.mv_cor.to.X][capture.mv_cor.to.Y];
    temp_board.board[capture.mv_cor.from.X][capture.mv_cor.from.Y] = Checker(ai_color);
    temp_board.board[midX][midY] = Checker(oponent_color);
}

