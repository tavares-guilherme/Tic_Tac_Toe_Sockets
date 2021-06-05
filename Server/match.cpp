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

bool Match::registerNewPlayer(int ip) {
    if (this->players.size() < 2) {
        Player newPlayer;

        newPlayer.ip = ip;

        if (this->players.size() == 0) {
            newPlayer.type = CROSS;
        } else {
            newPlayer.type = NOUGHT;

            this->currentPlayer = ip;
        }

        this->players.push_back(newPlayer);

        // Check if can init the match
        if (this->players.size() == 2)
            return true;
    }

    return false;
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
            foundPlayer.ip = currentPlayer.ip;
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

    if (winner != IN_PROGRESS)
        return foundPlayer.type;

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
    }

    // Check if Nought is the winner
    if(column_sum == 3 || row_sum == 3)
        return NOUGHT;

    
    // Check if Cross is the winner
    if(column_sum == -3 || row_sum == -3)
        return CROSS;

    // Checking for diagonals...
    if (diagonal_sum[0] == 3 || diagonal_sum[1] == 3)
        return NOUGHT_WIN;
    
    if (diagonal_sum[0] == -3 || diagonal_sum[1] == -3)
        return CROSS_WIN;
    
    // Chefk for draw
    if(this->remainingPositions == 0)
        return DRAW;

    return IN_PROGRESS;
}
