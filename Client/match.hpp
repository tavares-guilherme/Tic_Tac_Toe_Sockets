#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>

#define CROSS   'X'
#define NOUGHT  'O'

struct Position {
    int x;
    int y;
};

class Match {

    private:        

        /* Winner of the Match 
            -1 - Cross  player
             1 - Nought player
             3 - Draw
             4 - Match in progress
        */  
        int  winner;
        // Stores the number os free positions
        int remainingPositions;
        // Stores the board of the match
        char board[3][3];

    public:       
        
        Match();

        /* Prints the board in the terminal */
        void printBoard();

        void winnerMessage(char winner);
        void  setBoardPosition(Position, char player);
        int** getBoard();
        int   getWinner();

        Position   makePlay();

};

#endif