#include <iostream>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match(int _player) {
    
    // Sets all values of the board with 0(empty)
    memset(this->board, 0, sizeof(this->board));
    this->remainingPositions = 9;
    

    // TO-DO - Trow exception if != 1 && != -1
    this->player = _player;
}

void Match::printBoard() {
    
    for(int i = 0; i < 3; i++) {
        printf("-------------\n|");
        for(int j = 0; j < 3; j++) {
            
            printf(" %d |", this->board[i][j]);
        }
        printf("\n");
    }
    printf("-------------\n|");

    return;
}

void Match::defineWinner() {

    // Auxiliary variables to check the values...
    int column_sum      = 0;
    int row_sum         = 0;
    int diagonal_sum[2] = {0,0};

    for(int i = 0; i < 3; i++) {
        diagonal_sum[0] += this->board[i][i];
        diagonal_sum[1] += this->board[2-i][2-i];
        for(int j = 0; j < 3; j++) {
            column_sum += this->board[j][i];
            row_sum    += this->board[j][i];
        }

        // Check if Nought is the winner
        if(column_sum == 3 || row_sum == 3){
            this->winner = NOUGHT;
            return;
        }
        // Check if Cross is the winner
        if(column_sum == -3 || row_sum == -3){
            this->winner = CROSS;
            return;
        }

        if(remainingPositions == 0){
            this->winner = DRAW;
            return;
        }

        this->winner = IN_PROGRESS;
        return;
    }

    // Checking for diagonals...
    if(diagonal_sum[0] == 3 || diagonal_sum[1] == 3){
        this->winner = NOUGHT;
        return;
    }
    if(diagonal_sum[0] == -3 || diagonal_sum[1] == -3){
        this->winner = CROSS;
        return;
    }

}

int Match::makePlay() {
    
    int x, y;
    bool validPosition = false;

    if(this->player != CROSS && this->player != NOUGHT) {
        
        return 0;
    }

    while(!validPosition){

        cout << "Escolha uma posição em que deseja jogar (Ex: '1 2'): \n";
        cin >> x >> y;

        if( x>=1 && x<=3 && y>=1 && y<=3 && this->board[y][x] == 0)
            validPosition = true;
        else{
            validPosition = false;
            cout << "Posição inválida, tente novamente.\n";
        }
    }
    return 1;
}


