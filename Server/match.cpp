#include <iostream>
#include <stdio.h>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match() {
    // Sets all values of the board with 0(empty)
    memset(&this->board, 0, sizeof(&this->board));
    this->remainingPositions = 9;
}

void Match::printBoard() {
    
    for(int i = 0; i < 3; i++) {
        
        cout << " " << this->board[i][0] << " | " << board[i][1] << " | " << board[i][2] << endl;
        if(i < 2) cout << "-----------" << endl;
    }
}

bool Match::registerNewPlayer(int ip) {
    this->lock.lock();

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
        if (this->players.size() == 2) {
            this->lock.unlock();

            return true;
        }
    }

    this->lock.unlock();

    return false;
}

char Match::registerPlay(int player, int x, int y) {
    this->lock.lock();

    if (player != this->currentPlayer) {
        this->lock.unlock();

        return INVALID;
    }

    Player foundPlayer;
    int nextPlayer;

    cout << "[+] Registering play: " << player << " / " << x << " " << y << endl;

    for (int i = 0; i < this->players.size(); i++) {
        if (this->players[i].ip == this->currentPlayer) {
            foundPlayer.ip = this->players[i].ip;
            foundPlayer.type = this->players[i].type;
        } else {
            nextPlayer = this->players[i].ip;
        }
    }
    
    if (this->board[y][x] == CROSS || this->board[y][x] == NOUGHT) {
        this->lock.unlock();

        return INVALID_POSITION;
    }
    
    // 
    this->board[y][x] = foundPlayer.type;
    this->remainingPositions--;
    this->currentPlayer = nextPlayer;
    

    // Check if somebody has winned the match
    char winner = this->defineWinner();
    this->lock.unlock();

    if (winner == IN_PROGRESS)
        return foundPlayer.type;

    return winner;
}

char Match::defineWinner() {
    // Vai mandar um broadcast com o winner
    // Auxiliary variables to check the values...
    int column_sum      = 0;
    int row_sum         = 0;
    int diagonal_sum[2] = {0,0};

    // Atualizado para funcionar com um tabuleiro char[][]
    for(int i = 0; i < 3; i++) {
        
        // Checa as diagonais
        if (this->board[i][i] == NOUGHT) {
            diagonal_sum[0] += 1;
            diagonal_sum[1] += 1;
        } else if (this->board[i][i] == CROSS) {
            diagonal_sum[0] -= 1;
            diagonal_sum[1] -= 1;
        }
    }
    
    // Checking for diagonals...
    if (diagonal_sum[0] == 3 || diagonal_sum[1] == 3)
        return NOUGHT_WIN;
    
    if (diagonal_sum[0] == -3 || diagonal_sum[1] == -3)
        return CROSS_WIN;

    for (int i = 0; i < 3; i++) {
        int sum = 0;

        for (int j = 0; j < 3; j++) {
            if (this->board[i][j] == NOUGHT) {
                sum += 1;
            } else if (this->board[i][j] == CROSS) {
                sum -= 1;
            }
        }

        if (sum == 3) {
            return NOUGHT_WIN;
        } else if (sum == -3) {
            return CROSS_WIN;
        }
    }

    for (int i = 0; i < 3; i++) {
        int sum = 0;

        for (int j = 0; j < 3; j++) {
            if (this->board[j][i] == NOUGHT) {
                sum += 1;
            } else if (this->board[j][i] == CROSS) {
                sum -= 1;
            }
        }

        if (sum == 3) {
            return NOUGHT_WIN;
        } else if (sum == -3) {
            return CROSS_WIN;
        }
    }
    
    // Chefk for draw
    if(this->remainingPositions == 0)
        return DRAW;

    return IN_PROGRESS;
}

int Match::getRemainingPositions() {
    return this->remainingPositions;
}

int Match::getNextPlayer() {
    int ip = 0;

    this->lock.lock();
    ip = this->currentPlayer;
    this->lock.unlock();

    return ip;
}