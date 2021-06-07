#ifndef __MATCH
#define __MATCH

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include "../defines.hpp"

struct Position {
    int x;
    int y;
};

struct Player {
    int ip;
    char type; // Cross or nought
};

class Match {
    private:        
        int                 remainingPositions;
        int                 currentPlayerIP;
        char                board[3][3];
        std::mutex          lock;
        std::vector<Player> players;
    
    public:
        /**
         * @brief Inicializa todos os valores do tabuleiro com 0 e remainingPositions com 9
         */
        Match();
        /**
         * @brief Imprime o tabuleiro na tela
         */
        void printBoard();
        /**
         * @brief Registra um novo jogador - só permite 2 jogadores
         * 
         * @param ip O IP do novo jogador
         * 
         * @return true se já adicionou todos os jogadores ou false, caso contrário
         */
        bool registerNewPlayer(int ip);
        /**
         * @brief Pega o IP do próximo a jogar, armazenado em currentPlayer
         * 
         * @return um int, o IP do próximo jogador
         */
        int getNextPlayer();
        /**
         * @brief Registra a jogada do jogador, checa se ele é vitorioso ou se houve empate e atualiza o próximo jogador em currentPlayer
         * 
         * @param playerIP O ip do jogador atual
         * @param pos a posição escolhida pelo jogador
         * @return uma flag, de vencedor, empate, em progresso ou inválido
         */
        int registerPlay(int playerIP, Position pos);
        /**
         * @brief Verifica se a jogada atual de um jogador resultou numa vitória
         * 
         * @param pos a posição escolhida pelo jogador
         * @param type o tipo (X ou O) do jogador
         * @return true, se venceu, false, caso contrário
         */
        bool isWinner(Position pos, char type);
         /**
         * @brief Pega o valor da variável remainingPositions
         * 
         * @return o valor de remainingPositions
         */
        int getRemainingPositions();

};

#endif