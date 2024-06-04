//
// Created by bkami on 25.05.2024.
//

#include "board.h"


Board::Board() {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if ((i + j) % 2 != 0) {
                if (i < 3) {
                    board[i][j] = Checker(Color::WHITE);
                }
                else if (i > 4) {
                    board[i][j] = Checker(Color::BLACK);
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
bool Board::hasChecker(int x, int y) const{
    if(board[x][y].color!=Color::NONE){
        return true;
    }
    return false;
}
bool Board::isSafe(int x, int y, Color color) const {
    static const int dx[] = { -1, -1, 1, 1 };
    static const int dy[] = { -1, 1, -1, 1 };
    static const int captureDx[] = { -2, -2, 2, 2 };
    static const int captureDy[] = { -2, 2, -2, 2 };

    // Sprawdź, czy na polu (x, y) znajduje się pionek o podanym kolorze
    if (!hasChecker(x, y) || getChecker(x, y).color != color) {
        return true; // Jeśli nie ma pionka lub jest innego koloru, jest bezpieczny (nie ma pionka do przejęcia)
    }

    // Sprawdź, czy przeciwnik może zbić pionek
    Color opponentColor = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;

    for (int i = 0; i < 4; ++i) {
        int overX = x + dx[i];
        int overY = y + dy[i];
        int toX = x + captureDx[i];
        int toY = y + captureDy[i];
        Coordinates cor_sf = {{x,y},{toX, toY}};
        // Sprawdź, czy przeciwnik może przeskoczyć (x, y)
        if (isMoveValid(overX, overY, toX, toY) && hasChecker(overX, overY)) {
            Checker overChecker = getChecker(overX, overY);

            if (overChecker.color == opponentColor && canCapture(cor_sf)) {
                return false; // Pionek nie jest bezpieczny, przeciwnik może go zbić
            }
        }
    }

    return true; // Pionek jest bezpieczny
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
    if(board[fromX][fromY].color==Color::BLACK && !board[fromX][fromY].isQueen && toX > fromX){
        return false;
    }
    if(board[fromX][fromY].color==Color::WHITE && !board[fromX][fromY].isQueen && toX < fromX){
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
    if(board[cor.to.X][cor.to.Y].color==Color::BLACK && !board[cor.from.X][cor.from.Y].isQueen && cor.to.X >= cor.from.X){
        return false;
    }
    if(board[cor.to.X][cor.to.Y].color==Color::WHITE && !board[cor.from.X][cor.from.Y].isQueen && cor.to.X <= cor.from.X){
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



