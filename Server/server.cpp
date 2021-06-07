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

#define PORT 4445
#define DEBUG 1


/*
 *  Construtor da classe, seta os atributos do socket e do endereço do servidor.
 *  Invoca a thread que fará o servidor funcionar
 */
Server::Server() {

    // Define o socket
    int status;
    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Define o endereço do socket
    memset(&serverAddress, 0, sizeof(serverAddress));
    this->serverAddress.sin_family = AF_INET; // serverAddress
    this->serverAddress.sin_port   = htons(PORT); 
    this->serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    
    status = bind(this->serverSocket, (struct sockaddr*)&this->serverAddress, sizeof(this->serverAddress));
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
    
    this->lock.lock();
    int serverSocket = this->serverSocket;
    this->lock.unlock();

    int clientSocket = -1;
    socklen_t addrSize;
    struct sockaddr_in clientAddress;

    cout << "[+] Aguardando conexões " << endl;
    while (1) {
        
        // Aceita a conexão com um cliente
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &addrSize);

        if (clientSocket < 0) {
            // Retorna o programa caso haja algum erro de conexão.
            cout << "[-] Erro de conexão." << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << endl;

            close(clientSocket);

            this->lock.lock();
            this->serverIsOnline = false;
            this->lock.unlock();

            break;
        } else {
            cout << "[+] Conexão aceita de: " << atoi(inet_ntoa(clientAddress.sin_addr)) << ":" << ntohs(clientAddress.sin_port) << endl; 
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

    // Checa se a partida pode ser iniciada
    if (this->currentMatch.registerNewPlayer(atoi(inet_ntoa(address.sin_addr))) == true) {
        char type = (char) PacketType::RECEIVE_NEW_MATCH;
        char aux[2]  = {CROSS, NOUGHT};

        // Inicia o jogo para todos os jogadores
        for (int i = 0; i < this->playerSockets.size(); i++) {

            // O vetor Aux[i], sinalizará para cada jogador se ele é "NOUGHT" ou "CROSS"    
            sendPacket(type, aux[i], 0, this->playerSockets[i]);
        }

        type = (char) PacketType::ASK_POSITION;
        sendPacket(type, 0, 0, this->playerSockets[1]);
    }

    this->lock.unlock();

    // Create player listener thread
    this->playerThreads.push_back(thread(&Server::playerListener, this, clientSocket, address));
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

    while (1) {
        
        // Recebe a jogada do Cliente
        recv(clientSocket, buffer, sizeof(char) * 3, 0);
        if(DEBUG) cout << "[+] Pacote Recebido: " << (int) buffer[0] << "|" << (int) buffer[1] << " " << (int) buffer[2] << endl;

        currentPacket = receivePacket(buffer);

        if (currentPacket.type == (char) PacketType::SEND_POSITION) {
            // Registra a Jogada
            
            this->lock.lock();
            char result = this->currentMatch.registerPlay(atoi(inet_ntoa(clientAddress.sin_addr)), currentPacket.data1, currentPacket.data2);
            int nextPlayerIp = this->currentMatch.getNextPlayer();
            this->lock.unlock();

            if(DEBUG) {
                cout << "Result: " << (int) result << endl;
                this->currentMatch.printBoard();
            }

            // Envia a jogada para os outro jogadores e lida com os estados do sistema
            if (result == INVALID_POSITION) {

                // Posiçao inválida, envia o sinal de erro
                currentPacket.type = (char) PacketType::ASK_POSITION;
                sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, clientSocket);
            } else if (result == CROSS_WIN) {
                //  Recebe como ganhador o jogador CROSS
                this->lock.lock();
                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = CROSS;

                for (int i = 0; i < this->playerSockets.size(); i++) 
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                this->lock.unlock();
    
            } else if (result == NOUGHT_WIN) {
                // Recebe como ganhador o jogador NOUGHT

                // Método
                this->lock.lock();
                currentPacket.type = (char) PacketType::RECEIVE_WINNER;
                currentPacket.data1 = NOUGHT;

                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }
                this->lock.unlock();
            } else if (result == CROSS) {
                // Recebe a jogado de CROSS
                this->lock.lock();
                currentPacket.type = (char) PacketType::RECEIVE_POSITION_CROSS;

                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }

                for (int i = 0; i < this->playerSockets.size(); i++) {
                    if (atoi(inet_ntoa(this->playerAddresses[i].sin_addr)) == nextPlayerIp) {
                        currentPacket.type = (char) PacketType::ASK_POSITION;

                        sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                    }
                }
                this->lock.unlock();
            } else if (result == NOUGHT) {
                // Recebe a jogado de NOUGHT
                this->lock.lock();
                currentPacket.type = (char) PacketType::RECEIVE_POSITION_NOUGHT;

                for (int i = 0; i < this->playerSockets.size(); i++) {
                    sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                }

                for (int i = 0; i < this->playerSockets.size(); i++) {
                    if (atoi(inet_ntoa(this->playerAddresses[i].sin_addr)) == nextPlayerIp) {
                        currentPacket.type = (char) PacketType::ASK_POSITION;

                        sendPacket(currentPacket.type, currentPacket.data1, currentPacket.data2, this->playerSockets[i]);
                    }
                }
                this->lock.unlock();
            }
        }
    }
}
