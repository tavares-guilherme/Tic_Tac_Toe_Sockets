#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>

class Match {

    private:
        /* Stores the Board
        
            !! - It can be also stored as a simple array.
        */
        int board[3][3]; 

        /* Winner of the Match 
            1 - Player 1
            2 - Player 2
            3 - Draw
            4 - Match in progress
        */  
        int winner;

    public:
        
        int** getBoard();
        int   getWinner();

        /* Prints the bord in the terminal */
        void printBoard();

};

#endif