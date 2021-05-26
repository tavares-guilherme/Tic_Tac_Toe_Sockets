#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>

#define CROSS      -1
#define NOUGHT      1
#define DRAW        3
#define IN_PROGRESS 4

struct Position {
    char x;
    char y;
};

class Match {

    private:        

        /* Winner of the Match 
            -1 - Cross  player
             2 - Nought player
             3 - Draw
             4 - Match in progress
        */  
        int     remainingPositions;
        int     currentPlayer;
        int**   board;
        int**   players;

    public:

        Match();

        int defineWinner();

        void  registerPlay(int x, int y, int value);
        void  setBoard(int** _board);
        int** getBoard();
        int   getWinner();

        /* Prints the board in the terminal */
        void printBoard();

};

#endif