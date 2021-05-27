#include <stdio.h>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match() {
    this->board = (char**) malloc(sizeof(char) * 3);

    for(int i = 0; i < 3; i++)
        this->board[i] = (char*) malloc(sizeof(char));

    // Sets all values of the board with 0(empty)
    memset(this->board, 0, sizeof(this->board));
    this->remainingPositions = 9;
}

void Match::registerPlayer(int player) {
    this->lock.lock();

    if (this->players.size() < 2) {
        Player newPlayer;

        newPlayer.ip = players[i];

        if (this->player.size() == 0) {
            newPlayer.type = NOUGHT;
        } else {
            newPlayer.type = CROSS;
            this->currentPlayer = player;
        }
        
        this->players.push_back(newPlayer);

    }

    this->lock.unlock();
}

char Match::registerPlay(int player, int x, int y) {
    this->lock.lock();

    if (player != this->currentPlayer)
        return INVALID;

    Player foundPlayer;
    int nextPlayer;

    for (int i = 0; i < this->players.size(); i++) {
        Player currentPlayer = this->players[i];

        if (currentPlayer.ip == player) {
            foundPlayer.ip = ip;
            foundPlayer.type = currentPlayer.type;
        } else {
            nextPlayer = currentPlayer.ip;
        }
    }

    
    if (this->board[y][x] != '\0') {
        return INVALID;
    }
    else {
        this->board[y][x] = foundPlayer.type;
        this->remainingPositions--;
        this->currentPlayer = nextPlayer;
    }

    // Check if somebody has winned the match
    char winner = this->defineWinner();
    this->lock.unlock();

    return winner;
}

char Match::defineWinner() {
    // Vai mandar um broadcast com o winner
    // Auxiliary variables to check the values...
    int column_sum      = 0;
    int row_sum         = 0;
    int diagonal_sum[2] = {0,0};
    int winner;

    for(int i = 0; i < 3; i++) {
        diagonal_sum[0] += this->board[i][i];
        diagonal_sum[1] += this->board[2-i][2-i];

        for(int j = 0; j < 3; j++) {
            column_sum += this->board[j][i];
            row_sum    += this->board[j][i];
        }

        // Check if Nought is the winner
        if(column_sum == 3 || row_sum == 3){
            return NOUGHT;

        }
        // Check if Cross is the winner
        if(column_sum == -3 || row_sum == -3){
            return CROSS;

        }

        if(remainingPositions == 0){
            return DRAW;

        }

        winner = IN_PROGRESS;
    }

    // Checking for diagonals...
    if (diagonal_sum[0] == 3 || diagonal_sum[1] == 3)
        return NOUGHT;
    
    if (diagonal_sum[0] == -3 || diagonal_sum[1] == -3)
        return CROSS;
}

void Match::setBoard(int** _board) {
    if(sizeof(_board) != sizeof(int) * 9)
        return;

    this->board = _board;
}