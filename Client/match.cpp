#include <iostream>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match() {
    
    for(int i = 0; i < 9; i++) {
        this->board[i/3][i%3] = (char) i+49;
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
void Match::winnerMessage(char winner) {
    cout << "#--------------------------------------------#" << endl;
    cout << "  O jogador " << winner << " ganhou o jogo!" << endl;
    cout << "#--------------------------------------------#" << endl;
}

void Match::setBoardPosition(Position p, char player) {
    // Não precisa de verificação, x e y são dados a partir de um "MakePlay"
    this->board[p.y][p.x] = player;
}

Position Match::makePlay() {
    Position pos;
    int aux;
    bool validPosition = false;

    while (!validPosition) {
        this->printBoard();

        cout << "Escolha a posição livre que deseja jogar (Ex: 5)" << endl;
        cin >> aux;
        aux--;
        
        pos.y = aux/3;
        pos.x = aux%3;

        if (pos.x >= 0 && pos.x <= 3 && pos.y >= 0 && pos.y <= 3 && this->board[pos.y][pos.x] != CROSS && this->board[pos.y][pos.x] != NOUGHT) {
            validPosition = true;
        } else {
            validPosition = false;
            cout << "Posição inválida, tente novamente." << endl;
        }
    }

    return pos;
}


