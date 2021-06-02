#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <string.h>
#include <unistd.h>
#include "match.hpp"
#include "packet.hpp"
#include "server.hpp"


#define DEBUG 1

using namespace std;

// IP
// printf("Conexão aceita: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port))

/*
 *  Constutor da classe, seta os atributos do socket e do endereço do servidor.
 *  Invoca a thread que fará o servidor funcionar
 */
Server::Server() {
    // Define o socket
    int status;
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Define o endereço do socket
    this->server_address->sin_family = AF_INET;
    this->server_address->sin_port   = htons(8000); 
    this->server_address->sin_addr.s_addr = INADDR_ANY;
    memset(&this->server_address->sin_zero, 0, sizeof(this->server_address->sin_zero));
    
    status = bind(this->serverSocket, (struct sockaddr*) this->server_address, sizeof(this->server_address));
    cout << "Bind: " << status << "\n";
    status = listen(this->serverSocket, 2);
    cout << "listen: " << status << "\n";

    this->serverThread = thread(&Server::waitForConnection, this);

    this->serverThread.join();
}

/*
 *  Função que será utilizada como thread, 
 *  Serve para aguardar as conexões com o servidor. 
 */
void Server::waitForConnection() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;


    while (1) {
        // Aceita a conexão com um cliente
        client_socket = accept(serverSocket, (struct sockaddr*)&client_address, (socklen_t*) sizeof(client_address));

        if(client_socket < 0){
            // Retorna o programa caso haja algum erro de conexão.

            printf("Erro de conexão.");
            return;
        }else {
            // Para de "escutar" o socket e inicia as connexões
            close(this->serverSocket);
            this->registerNewConnection(client_socket, client_address);
        }
    }
}

/*
 *  Cadastra no sistema do servidor ma nova conxão
 */
void Server::registerNewConnection(int clientSocket, struct sockaddr_in address) {
    this->lock.lock();

    PlayerSocket newPlayer;

    newPlayer.socket = clientSocket;
    newPlayer.address = &address;
    newPlayer.thread = thread(&Server::playerListener, this, clientSocket, address);

    this->players.push_back(newPlayer);
    this->lock.unlock();
}

void Server::playerListener(int clientSocket, struct sockaddr_in clientAddress) {
    
    Packet currentPacket;
    char buffer[3];

    while (1) {
        // Recebe a jogada do Cliente
        recv(clientSocket, buffer, sizeof(char) * 3, 0);

        currentPacket = receivePacket(buffer);

        if (currentPacket.type == (char) PacketType::SEND_POSITION) {
            
            // Registra a Jogada
            char result = this->currentMatch.registerPlay(atoi(inet_ntoa(clientAddress.sin_addr)), currentPacket.data1, currentPacket.data2);
            
            // Envia a jogada para os outro jogadores e lida com os estados do sistema
            if (result == INVALID_POSITION) {
                // Posiçao inválida, envia o sinal de erro

                currentPacket.type = (char) PacketType::ASK_POSITION;
                sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, clientSocket);
            } else if (result == CROSS) {
                // Última jogada de "Cross", envia a jogada para ambos jogadores

                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = CROSS;
                
                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) 
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->players[i].socket);
    
                this->lock.unlock();
            } else if (result == NOUGHT) {
                // Última jogada de "Noought", envia a jogada para ambos jogadores

                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = NOUGHT;

                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->players[i].socket);
                }
                this->lock.unlock();
            } else {
                // Servidor está no estado de receber uma jogada

                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) {
                    
                    // Checa o endereço do jogador antes de realizar as jogadas
                    if (strcmp(inet_ntoa(this->players[i].address->sin_addr), inet_ntoa(clientAddress.sin_addr)) == 0) {
                        if (currentPacket.type == NOUGHT)
                            currentPacket.type = (char) PacketType::RECEIVE_POSITION_NOUGHT;
                        else
                            currentPacket.type = (char) PacketType::RECEIVE_POSITION_CROSS;
                    }
                }

                // Envia os resultados
                for (int i = 0; i < this->players.size(); i++) 
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->players[i].socket);
    

                this->lock.unlock();
            }
        }
    }
}
