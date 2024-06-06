//
// Created by bkami on 25.05.2024.
//

#include "board.h"


Board::Board() {
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
//bool Board::getIfQueen(int x, int y) const{
//    return board[x][y].isQueen;
//}
Checker Board::getChecker(int x, int y) const {
    return board[x][y];
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
    for (int i=0; i<4; i++) {
        int newX = x + captureDirections[i].X;
        int newY = y + captureDirections[i].Y;
        Coordinates cor = {{x, y}, {newX, newY}};
        if (canCapture(cor)) {
            moves.push_back({cor, true});
        }
    }

    //generateMultipleCapture(x,y,moves);
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

Coordinates Board::changeCoordinates(int from, int to) {
    Coordinates cor;
    int ind;
    ind=from-1;
    cor.from.X= ind/4;
    if(cor.from.X%2==0){
        cor.from.Y=(ind%4)*2 +1;
    } else{
        cor.from.Y = (ind % 4)*2;
    }
    ind=to-1;
    cor.to.X= ind/4;
    if(cor.to.X%2==0){
        cor.to.Y=(ind%4)*2 +1;
    } else{
        cor.to.Y = (ind % 4)*2;
    }
    return cor;
}
bool Board::moveChecker(Coordinates move_cor) {
    //Coordinates move_cor = changeCoordinates(from, to);
    if (!isMoveValid(move_cor.from.X, move_cor.from.Y, move_cor.to.X, move_cor.to.Y)) {
        return false;
    }



    board[move_cor.to.X][move_cor.to.Y] = board[move_cor.from.X][move_cor.from.Y];
    board[move_cor.from.X][move_cor.from.Y] = Checker();
    QueenCheck(move_cor);
    return true;
}
bool Board::QueenCheck(Coordinates cor_que) {
    //////////////////zamiana w krolowa///////////////////////////
    if(board[cor_que.to.X][cor_que.to.Y].color == Color::BLACK && cor_que.to.X == 0){
        board[cor_que.to.X][cor_que.to.Y].isQueen = true;
        std::cout<<"to       "<<board[cor_que.to.X][cor_que.to.Y].isQueen<<std::endl;
        return true;
    }
    if(board[cor_que.to.X][cor_que.to.Y].color == Color::WHITE && cor_que.to.X == BOARD_SIZE-1){
        board[cor_que.to.X][cor_que.to.Y].isQueen = true;
        return true;
    }
    return false;
    //////////////////////////////////////////////////////////////
}
bool Board::canCapture(Coordinates cor) const {
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
    if (board[cor.to.X][cor.to.Y].color != Color::NONE) {
        return false;
    }

    int midX = (cor.from.X + cor.to.X) / 2;
    int midY = (cor.from.Y + cor.to.Y) / 2;

    if (board[midX][midY].color == Color::NONE || board[midX][midY].color == board[cor.from.X][cor.from.Y].color) {
        return false;
    }
    if(board[cor.from.X][cor.from.Y].color==Color::WHITE && !board[cor.from.X][cor.from.Y].isQueen && cor.to.X >= cor.from.X){
        return false;
    }
    if(board[cor.from.X][cor.from.Y].color==Color::BLACK && !board[cor.from.X][cor.from.Y].isQueen && cor.to.X <= cor.from.X){
        return false;
    }


    return true;
}
bool Board::Capture(Coordinates capt_cor) {
    Position captured;
    //Coordinates capt_cor = changeCoordinates(from, to);
    if(!canCapture(capt_cor)){
        return false;
    }
    int midX = (capt_cor.from.X + capt_cor.to.X)/2;
    int midY = (capt_cor.from.Y + capt_cor.to.Y)/2;



    board[capt_cor.to.X][capt_cor.to.Y] = board[capt_cor.from.X][capt_cor.from.Y];
    board[capt_cor.from.X][capt_cor.from.Y] = Checker();
    board[midX][midY] = Checker();
    QueenCheck(capt_cor);

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

//void Board::generateMultipleCapture(int x, int y, std::vector<Move>& moves){
//    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
//    for (int i=0; i<4; i++) {
//        int newX = x + captureDirections[i].X;
//        int newY = y + captureDirections[i].Y;
//        //Coordinates cor = {{x, y}, {newX, newY}};
//        std::vector<Position> capt_cor = {{x,y},{newX,newY}};
//        if(canCapture(capt_cor)){
//            moves.push_back({capt_cor, true});
//        }
//        if(QueenCheck(capt_cor)){
//            return;
//        }
//        std::vector<Move> moves_mult_cap;
//        generateCapturesForChecker(capt_cor.back().X,capt_cor.back().Y, moves_mult_cap);
//        if(!moves_mult_cap.size()){
//            return;
//        } else {
//            Position current_end_moves_mult = {moves_mult_cap.front().mv_cor.back().X,
//                                               moves_mult_cap.front().mv_cor.back().Y}; //zawsze bierzemy pierwsze mozliwe bicie wielokrotne
//            Position old_end_moves_mult = {moves[i].mv_cor.back().X, moves[i].mv_cor.back().Y};
//            std::vector<Position> mult_cap;// = {moves.front().mv_cor,moves_mult_cap,end_moves_mult};
//            mult_cap.push_back({moves[i].mv_cor.front().X,moves[i].mv_cor.front().X});
//            mult_cap.push_back(old_end_moves_mult);
//            mult_cap.push_back(current_end_moves_mult);
//            moves.pop_back();
//            moves.push_back({mult_cap, true});
//            generateMultipleCapture(current_end_moves_mult.X,current_end_moves_mult.Y,moves);
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

void Board::available_jump_sequences(Coordinates cor_cap, std::vector<Coordinates> current_sequence,
                                     Checker temp_board[BOARD_SIZE][BOARD_SIZE],
                                     std::vector<Coordinates> &sequences) {
    std::vector<Move> caps = available_cap_from(, temp_board);
    if (caps.empty()) {
        sequences.push_back(current_sequence);
        return;
    }
    //for (const Position &j : cap) {
    for(int i=0;i<caps.size();i++){
        Checker new_board[BOARD_SIZE][BOARD_SIZE];
        for(int j=0;j<BOARD_SIZE;j++){
            for (int k = 0; k < BOARD_SIZE; ++k) {
                new_board[j][k]=temp_board[j][k];
            }
        }
        Coordinates current_cor = {{from},{caps[i].mv_cor.to}};
        cap(current_cor, new_board);
        std::vector<Position> new_sequence = current_sequence;
        new_sequence.push_back(caps[i].mv_cor.to);
        available_jump_sequences(caps[i].mv_cor.to, new_sequence, new_board, sequences);
    }
}

std::vector<Move> Board::available_cap_from(Position from, Checker temp_board[BOARD_SIZE][BOARD_SIZE]) const {
    std::vector<Move> caps;
    static const Position captureDirections[4] = {{-2, -2}, {-2, 2}, {2, -2}, {2, 2}};
    for (int i=0; i<4; i++) {
        int newX = from.X + captureDirections[i].X;
        int newY = from.Y + captureDirections[i].Y;
        Coordinates cor = {{from.X, from.Y}, {newX, newY}};
        if (canCapture(cor)) {
            caps.push_back({cor, true});
        }
    }
    return caps;
}

void Board::cap(Coordinates &capt_cor, Checker temp_board[BOARD_SIZE][BOARD_SIZE]) {
    int midX = (capt_cor.from.X + capt_cor.to.X) / 2;
    int midY = (capt_cor.from.Y + capt_cor.to.Y) / 2;
    temp_board[capt_cor.to.X][capt_cor.to.Y] = temp_board[capt_cor.from.X][capt_cor.from.Y];
    temp_board[capt_cor.from.X][capt_cor.from.Y] = Checker();
    temp_board[midX][midY] = Checker();
}


