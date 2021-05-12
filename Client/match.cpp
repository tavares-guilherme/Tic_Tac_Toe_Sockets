#include <stdio.h>
#include "match.hpp"

using namespace std;

void Match::printBoard() {
    
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            
            printf("%d ", this->board[i][j]);
        }
        printf("\n");
    }

    return;
}