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
    this->server_address.sin_family = AF_INET; // serverAddress
    this->server_address.sin_port   = htons(8000); 
    this->server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    memset(&this->server_address.sin_zero, 0, sizeof(this->server_address.sin_zero));
    
    status = bind(this->serverSocket, (struct sockaddr*)&this->server_address, sizeof(this->server_address));
    cout << "Bind: " << status << "\n";
    if(status < 0) return;
    status = listen(this->serverSocket, 100);
    cout << "listen: " << status << "\n";
    if(status < 0) return;

    // Init server thread
    this->serverIsOnline = true;

    this->serverThread = thread(&Server::waitForConnection, this);
    this->serverThread.join();

    for (int i = 0; i < this->playerThreads.size(); i++) {
        this->playerThreads[i].join();
    }
}

bool Server::online() {
    bool value = false;
    this->lock.lock();
    value = this->serverIsOnline;
    this->lock.unlock();

    return value;
}

/*
 *  Função que será utilizada como thread, 
 *  Serve para aguardar as conexões com o servidor. 
 */
void Server::waitForConnection() {
    int clientSocket = -1;
    socklen_t addrSize;
    struct sockaddr_in clientAddress;
    cout << "Aguardando pelas conexões" << endl;


    while(1) {
        
        this->lock.lock();
        // Aceita a conexão com um cliente
        clientSocket = accept(this->serverSocket, (struct sockaddr*)&clientAddress, &addrSize);
        this->lock.unlock();

        if (clientSocket < 0){
            // Retorna o programa caso haja algum erro de conexão.
            cout << "Ocorreu um erro de conexão" << endl;

            this->lock.lock();
            this->serverIsOnline = false;
            this->lock.unlock();

            break;
        } else {

            cout << "Um novo socket foi conectado" << endl;
            this->registerNewConnection(clientSocket, clientAddress);
        }
    }

    this->lock.lock();
    close(this->serverSocket);
    this->lock.unlock();
}

/*
 *  Cadastra no sistema do servidor ma nova conxão
 */
void Server::registerNewConnection(int clientSocket, struct sockaddr_in address) {
    this->lock.lock();

    //  Definição de seus atributos
    this->playerSockets.push_back(clientSocket);
    this->playerAddresses.push_back(address);

    // Check if can init the match
    if (this->currentMatch.registerNewPlayer(atoi(inet_ntoa(address.sin_addr)))) {
        char type = (char) PacketType::RECEIVE_NEW_MATCH;
        // Init new match for all players
        for (int i = 0; i < this->playerSockets.size(); i++) {
            sendPacket(type, 0, 0, this->playerSockets[i]);
        }

        // Ask position for player
        type = (char) PacketType::ASK_POSITION;
        sendPacket(type, 0, 0, clientSocket);
    }

    // Create player listener thread
    this->playerThreads.push_back(thread(&Server::playerListener, this, clientSocket, address));
    this->lock.unlock();
}

/*
 *  Função que lida com a conexão com os jogadores durante o jogo.
 *  Para que ocorra uma partida, essa função deve ser invocada como uma thread
 */ 
void Server::playerListener(int clientSocket, struct sockaddr_in clientAddress) {
    
    // Auxilia na transmissão de dados
    Packet currentPacket;
    // Buffer para receber as informações dos clientes
    char buffer[3];

    while (this->online()) {
        
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
            } else if (result == CROSS_WIN) {
                //  Recebe como ganhador o jogador CROSS

                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = CROSS;
                
                this->lock.lock();
                for (int i = 0; i < this->playerSockets.size(); i++) 
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
    
                this->lock.unlock();
            } else if (result == NOUGHT_WIN) {
                // Recebe como ganhador o jogador NOUGHT

                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = NOUGHT;

                this->lock.lock();
                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }
                this->lock.unlock();
            } else if (result == CROSS) {
                // Recebe a jogado de CROSS

                currentPacket.type = (char) PacketType::RECEIVE_POSITION_CROSS;

                this->lock.lock();
                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }
                this->lock.unlock();
            } else if (result == NOUGHT) {
                // Recebe a jogado de NOUGHT

                currentPacket.type = (char) PacketType::RECEIVE_POSITION_NOUGHT;

                this->lock.lock();
                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }
                this->lock.unlock();
            }

            /*
            else {
                
                // Servidor está no estado de receber uma jogada
                this->lock.lock();
                for (int i = 0; i < this->playerSockets.size(); i++) {
                    
                    // Checa o endereço do jogador antes de realizar as jogadas
                    if (strcmp(inet_ntoa(this->playerAddresses[i].sin_addr), inet_ntoa(clientAddress.sin_addr)) == 0) {
                        if (currentPacket.type == NOUGHT)
                            currentPacket.type = (char) PacketType::RECEIVE_POSITION_NOUGHT;
                        else
                            currentPacket.type = (char) PacketType::RECEIVE_POSITION_CROSS;
                    }
                }

                // Envia os resultados
                for (int i = 0; i < this->playerSockets.size(); i++) 
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
    
                this->lock.unlock();
                
            }
            */
        }
    }
}
