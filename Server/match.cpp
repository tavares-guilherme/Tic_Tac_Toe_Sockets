#include <iostream>
#include <stdio.h>
#include <string.h>
#include "match.hpp"

using namespace std;

Match::Match() {
    memset(&this->board, 0, sizeof(this->board));
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

            this->currentPlayerIP = ip;
        }

        this->players.push_back(newPlayer);

        // Checa se já pode começar a partida
        if (this->players.size() == 2) {
            this->lock.unlock();

            return true;
        }
    }

    this->lock.unlock();

    return false;
}

int Match::registerPlay(int playerIP, Position pos) {
    this->lock.lock();

    if (playerIP != this->currentPlayerIP) {
        this->lock.unlock();

        return INVALID;
    }

    Player foundPlayer;
    int nextPlayerIP;

    cout << "[+] Registering play: " << playerIP << " / " << pos.x << " " << pos.y << endl;

    //search player
    for (int i = 0; i < (int) this->players.size(); i++) {
        if (this->players[i].ip == this->currentPlayerIP) {
            foundPlayer.ip = this->players[i].ip;
            foundPlayer.type = this->players[i].type;
        } else {
            nextPlayerIP = this->players[i].ip;
        }
    }
    
    if (this->board[pos.x][pos.y] == CROSS || this->board[pos.x][pos.y] == NOUGHT) {
        this->lock.unlock();

        return INVALID_POSITION;
    }
    
    this->board[pos.x][pos.y] = foundPlayer.type;
    this->remainingPositions--;
    this->currentPlayerIP = nextPlayerIP;

    // Checando se a atual jogada do jogador resulta numa vitória
    if (this->isWinner(pos, foundPlayer.type)){
        this->lock.unlock();
        return foundPlayer.type == CROSS ? CROSS_WIN : NOUGHT_WIN;
    }

    // Checando empate
    if(this->remainingPositions == 0){
        this->lock.unlock();
        cout << "EMPATE\n";
        return DRAW;
    }
        
    this->lock.unlock();
    return foundPlayer.type;
}

bool Match::isWinner(Position pos, char type) {
    // Vai mandar um broadcast com o winner
    int i = 0, j = 0;

    //Verificando a orientação vertical
    for (i = 0; i < 3; i++){
        if (this->board[i][pos.y] != type){
            break;
        }
    }

    if (i == 3) return true;

    //Verificando a orientação horizontal
    for (j = 0; j < 3; j++){
        if (this->board[pos.x][j] != type){
            break;
        }
    }

    if (j == 3) return true;

    // Verificando a diagonal direita
    for(i = 0; i < 3; i++) {
        if (this->board[i][i] != type){
            break;
        }
    }

    if (i == 3) return true;

    //Verificando a diagonal esquerda
    for (i = 0, j = 2; i < 3; i++, j--){
        if (this->board[i][j] != type){
            break;
        }
    }

    if (i == 3) return true;
    
    return false;
}

int Match::getRemainingPositions() {
    return this->remainingPositions;
}

int Match::getNextPlayer() {
    int ip = 0;

    this->lock.lock();
    ip = this->currentPlayerIP;
    this->lock.unlock();

    return ip;
}