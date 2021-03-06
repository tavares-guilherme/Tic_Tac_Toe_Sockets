#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>

struct Position {
    int x;
    int y;
};

class Match {
    private:        
        // Armazena o número de posições livres
        int remainingPositions;
        // Armazena o tabuleiro do jogo
        char board[3][3];
        // Armazena se o jogador é CROSS ou NOUGHT
        char player;
        // Armazena se o jogo acabou ou está em progresso
        bool gameInProgress;

    public:       
        /**
         * @brief Inicializa um tabuleiro com números de 1 a 9
         */
        Match();
        Match(char player);
        /**
         * @brief Imprime o tabuleiro na tela
         */
        void printBoard();
        /**
         * @brief Imprime quem venceu na tela
         * 
         * @param winner O vencedor da partida, X ou O
         */
        void printWinnerMessage(char winner);
        /**
         * @brief Verifica se uma posição está dentro do tabuleiro e se é livre
         * 
         * @param pos A posição a ser validada
         * 
         * @return true se é uma posição válida, ou false caso contrário
         */
        bool isValidPosition(Position pos);
        /**
         * @brief Faz uma jogada, lendo uma posição do teclado até que seja uma posição válida e livre
         * 
         * @return A posição lida
         */
        Position makeMove();
        /**
         * @brief Modifica o valor do tabuleiro numa determinada posição y (linha) e x (coluna)
         * 
         * @param p A posição do tabuleiro previamente validada
         * @param player O jogador atual, representado com X ou O
         */
        void setBoardPosition(Position p, char player);
        bool inProgress();
};

#endif