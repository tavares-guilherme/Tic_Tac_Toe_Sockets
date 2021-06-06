#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>

#define CROSS               'X'
#define NOUGHT              'O'
#define CROSS_WIN           3
#define NOUGHT_WIN          4
#define DRAW                5
#define IN_PROGRESS         6
#define INVALID             7
#define INVALID_POSITION    8

struct Position {
    char x;
    char y;
};

struct Player {
    int ip;
    char type; // Cross or nought
};

class Match {
    private:        
        int                 remainingPositions;
        int                 currentPlayer;
        char**              board;
        std::mutex          lock;
        std::vector<Player> players;
    
    public:

        Match();
        
        bool registerNewPlayer(int ip);

        int getNextPlayer();

        char registerPlay(int player, int x, int y); // char player, Position p
        char defineWinner();

        char** getBoard();
        /* Prints the board in the terminal */
        void printBoard();

};

#endif