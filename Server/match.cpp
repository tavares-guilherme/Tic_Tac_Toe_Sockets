#include <iostream>
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

    cout << "[+] Registering player: " << player << " / " << x << " " << y << endl;

    for (int i = 0; i < this->players.size(); i++) {
        if (this->players[i].ip == player) {
            foundPlayer.ip = this->players[i].ip;
            foundPlayer.type = this->players[i].type;
        } else {
            nextPlayer = this->players[i].ip;
        }
    }

    cout << "2" << endl;
    
    if (this->board[y][x] != '\0')
        return INVALID_POSITION;
    
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
        if(this->board[i][i] == NOUGHT){
            diagonal_sum[0] += 1;
            diagonal_sum[1] += 1;
        }else{
            diagonal_sum[0] -= 1;
            diagonal_sum[1] -= 1;
        }

        for(int j = 0; j < 3; j++) {
            if(this->board[j][i] == NOUGHT) {
                column_sum += 1;
                row_sum    += 1;
            }else{
                column_sum -= 1;
                row_sum    -= 1;
            }
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