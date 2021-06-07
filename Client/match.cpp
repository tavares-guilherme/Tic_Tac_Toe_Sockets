#include <iostream>
#include <string.h>
#include "match.hpp"
#include "../defines.hpp"

using namespace std;

Match::Match() {
    for(int i = 0; i < 9; i++) {
        this->board[i/3][i%3] = (char) i + '1';
    }
}

// Melhorar o visual
void Match::printBoard() {
    for(int i = 0; i < 3; i++) {
        cout << " " << this->board[i][0] << " | " << board[i][1] << " | " << board[i][2] << endl;
        if(i < 2) cout << "-----------" << endl;
    }
}

// Printar bonito
void Match::printWinnerMessage(char winner) {
    cout << "#--------------------------------------------#" << endl;
    cout << "  O jogador " << winner << " ganhou o jogo!" << endl;
    cout << "#--------------------------------------------#" << endl;
}

void Match::setBoardPosition(Position p, char player) {
    // Não precisa de verificação, x e y são dados a partir de um "MakeMove"
    this->board[p.x][p.y] = player;
}

bool Match::isValidPosition(Position pos){
    return pos.x >= 0 && pos.x <= 3 && pos.y >= 0 && pos.y <= 3 //está dentro dos limites dos tabuleiros
        && this->board[pos.x][pos.y] != CROSS && this->board[pos.x][pos.y] != NOUGHT; //não está preenchida ainda
}

Position Match::makeMove() {
    Position pos;
    int aux;
    bool validPosition;

    do {
        this->printBoard();

        cout << "Escolha a posição livre que deseja jogar (1 a 9):" << endl;
        cin >> aux;
        aux--; //índices vão de 0 a 8

        pos.x = aux/3;
        pos.y = aux%3;

        validPosition = isValidPosition(pos);

        if (!validPosition){
            cout << "Posição inválida, tente novamente." << endl;
        }
    } while (!validPosition);

    return pos;
}


