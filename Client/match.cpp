#include <iostream>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match() {
    
    // Sets all values of the board with 0(empty)
    memset(this->board, 0, sizeof(this->board));
}

// Melhorar o visual
void Match::printBoard() {
    for(int i = 0; i < 3; i++) {
        cout << "-------------" << endl;

        for(int j = 0; j < 3; j++)
            cout << this->board[i][j] << " |";
        cout << endl;
    }

    cout << "-------------|" << endl;
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
    bool validPosition = false;

    while (!validPosition) {

        cout << "Escolha uma posição em que deseja jogar (Ex: '1 2'):" << endl;
        cin >> pos.x >> pos.y;

        if (pos.x >= 1 && pos.x <= 3 && pos.y >= 1 && pos.y <= 3 && this->board[pos.y][pos.x] == 0) {
            validPosition = true;
        } else {
            validPosition = false;
            cout << "Posição inválida, tente novamente." << endl;
        }
    }

    return pos;
}


