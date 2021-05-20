#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>

#define CROSS      -1
#define NOUGHT      1
#define DRAW        3
#define IN_PROGRESS 4


class Match {

    private:        
        // Contains with the local player is playing with CROSS or NOUGHT
        int player;

        /* Winner of the Match 
            -1 - Cross  player
             2 - Nought player
             3 - Draw
             4 - Match in progress
        */  
        int  winner;
        int remainingPositions;

    public:
         /* Stores the Board
        
            !! - It can be also stored as a simple array.
        */
        int board[3][3];
        
        Match(int _player);

        void  defineWinner();

        int** getBoard();
        int   getWinner();

        int   makePlay();

        /* Prints the bord in the terminal */
        void printBoard();

};

#endif