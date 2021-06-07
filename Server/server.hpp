#ifndef __SERVER
#define __SERVER

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <mutex>
#include <vector>
#include "match.hpp"
#include "../packet.hpp"
#include "../defines.hpp"

class Server {
    
    private:
        int serverSocket;
        std::mutex lock;
        Match currentMatch;
        bool serverIsOnline = false;
    
    public:

        std::thread                     serverThread;

        std::vector<int>                playerSockets;
        std::vector<struct sockaddr_in> playerAddresses;
        std::vector<std::thread>        playerThreads;

        struct sockaddr_in serverAddress;
        /**
         * @brief Construtor da classe, seta os atributos do socket e do endereço do servidor
         * Invoca a thread que fará o servidor funcionar
         */
        Server();
        /**
         * @brief Função que será utilizada como thread. Serve para aguardar as conexões com o servidor.
         */
        void waitForConnection();
        /**
         * @brief Cadastra no sistema do servidor ma nova conexão
         * 
         * @param clientSocket o socket do cliente
         * @param address endereço do cliente
         */
        void registerNewConnection(int clientSocket, struct sockaddr_in address);
        /**
         * @brief Função que lida com a conexão com os jogadores durante o jogo.
         * Para que ocorra uma partida, essa função deve ser invocada como uma thread
         * 
         * @param clientSocket o socket do cliente
         * @param address endereço do cliente
         */
        void playerListener(int clientSocket, struct sockaddr_in address);
        /**
         * @brief Pega o valor de serverIsOnline.
         * 
         * @return se o servidor está online.
         */
        bool online();
        /**
         * @brief Envia pacote de vencedor para todos os sockets clientes.
         */
        void sendWinner(char winner);
        /**
         * @brief Envia requisição de posição para o próximo jogador.
         * 
         * @param packet o pacote que vai ser enviado.
         * @param nextPlayerIP o ip do próximo jogador.
         */
        void sendAskForPosition(Packet packet, int nextPlayerIP);
};
#endif
    