#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>

#define CROSS               'X'
#define NOUGHT              'O'
#define DRAW                3
#define IN_PROGRESS         4
#define INVALID             5
#define INVALID_POSITION    6

struct Position {
    char x;
    char y;
};

struct Player {
    int socket;
    int ip;
    char type; // Cross or nought
};

class Match {
    private:        
        int                 remainingPositions;
        char                currentPlayer;
        char**              board;
        std::mutex          lock;
        std::vector<Player> players;
    
    public:
        Match();

        char registerPlay(int player, int x, int y); // char player, Position p
        char defineWinner();

        char** getBoard();
        /* Prints the board in the terminal */
        void printBoard();

};

#endif