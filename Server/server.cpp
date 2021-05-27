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

Server::Server() {
    // Define o socket
    int status;
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Define o endereço do socket
    this->server_address.sin_family = AF_INET;
    this->server_address.sin_port   = htons(8000); 
    this->server_address.sin_addr.s_addr = INADDR_ANY;
    memset(&this->server_address.sin_zero, 0, sizeof(this->server_address.sin_zero));
    
    
    status = bind(this->serverSocket, (struct sockaddr*) &this->server_address, sizeof(this->server_address));
    status = listen(this->serverSocket, 2);

    this->serverThread = thread(this->listener());
}

void Server::listener() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;

    while (1) {
        client_socket = accept(serverSocket, (struct sockaddr*)&client_address, &addr_size);

        if(client_socket < 0)
            return 0;
        else {
            close(this->serverSocket);
            this->registerNewConnection(client_socket, client_address);
        }
    }
}

void Server::registerNewConnection(int clientSocket, struct sockaddr_in address) {
    this->lock.lock();

    PlayerSocket newPlayer;

    newPlayer.socket = clientSocket;
    newPlayer.address = address;

    // Create thread
    newPlayer.thread = std::thread(this->playerListener, clientSocket, address);

    this->players.push_back(newPlayer);

    this->lock.unlock();
}

void Server::playerListener(int clientSocket, struct sockaddr_in address) {
    
    Packet currentPacket;
    char buffer[3];

    while (1) {
        // Recebe a jogada do Cliente
        recv(clientSocket, buffer, sizeof(char) * 3, 0);

        currentPacket = receivePacket(buffer);

        if (currentPacket.type == PacketType::SEND_POSITION) {
            
            // Registra a Jogada
            char result = m->registerPlay(atoi(inet_ntoa(client_address.sin_addr)), currentPacket.data1, currentPacket.data2));
            
            // Envia a jogada para os outro jogadores
            if (result == INVALID_POSITION) {
                currentPacket.type = ASK_POSITION;

                sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, clientSocket);
            } else if (result == CROSS) {
                currentPacket.type = RECEIVE_WINNER;
                currentPacket.data1 = CROSS;
                
                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) {
                    PlayerSocket currentPlayer = this->players[i];

                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, currentPlayer.socket);
                }
                this->lock.unlock();
            } else if (result == NOUGHT) {
                currentPacket.type = RECEIVE_WINNER;
                currentPacket.data1 = NOUGHT;

                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) {
                    PlayerSocket currentPlayer = this->players[i];

                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, currentPlayer.socket);
                }
                this->lock.unlock();
            } else if (result != INVALID) {
                this->lock.lock();
                for (int i = 0; i < this->players.size(); i++) {
                    PlayerSocket currentPlayer = this->players[i];

                    if (currentPlayer.ip == address) {
                        if (currentPlayer.type == NOUGHT)
                            currentPacket.type = RECEIVE_POSITION_NOUGHT;
                        else
                            currentPacket.type = RECEIVE_POSITION_CROSS;
                    }
                }

                for (int i = 0; i < this->players.size(); i++) {
                    PlayerSocket currentPlayer = this->players[i];

                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, currentPlayer.socket);
                }

                this->lock.unlock();
            }
        }
    }
}
int main() {

    // Client sockets address
    Server s;
    return 0;
}
