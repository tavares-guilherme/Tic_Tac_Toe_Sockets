#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include "match.hpp"
#include "../packet.hpp"
#include "../defines.hpp"

using namespace std;

int main() {

    Match m;
    char buffer[3];
    int status;
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in clientAddress;
    // Socket address definitions
    memset(&clientAddress, 0, sizeof(clientAddress));
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port   = htons(PORT);
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);

   status = connect(clientSocket, (struct sockaddr*)&clientAddress, sizeof(clientAddress));

    if (status < 0) {
        cout << "Falha ao conectar-se com o servidor. Status: " << status << endl;

        exit(0);
    }
    cout << "Conectado com sucesso." << endl;

    while(m.inProgress()) {
        Packet currentPacket;

        // Recebe a mensagem do servidor
        status = recv(clientSocket, buffer, sizeof(char) * 3, 0);

        currentPacket = receivePacket(buffer);
        
        if(status < 0) { // Status de Erro
            cout << "Erro ao receber dados so servidor." << endl;

            return 1;
        }

        Position position;
        position.x = (int) currentPacket.data1;
        position.y = (int) currentPacket.data2;
        
        switch (currentPacket.type) {
        
            case (char) PacketType::RECEIVE_NEW_MATCH: 
                // Início da Partida
                m = Match(currentPacket.data1);
                cout << "[+] Início da partida, você jogará com: " << currentPacket.data1 << endl;
                break;

            case (char) PacketType::ASK_POSITION:
                
                // Servidor Solicitando jogada
                position = m.makeMove();
                currentPacket.data1 = position.x;
                currentPacket.data2 = position.y;

                sendPacket({(char) PacketType::SEND_POSITION, currentPacket.data1, currentPacket.data2}, clientSocket);

                break;

            case (char) PacketType::RECEIVE_POSITION_CROSS:
                // Servidor enviando jogada
                m.setBoardPosition(position, CROSS);
                break;
            
            case (char) PacketType::RECEIVE_POSITION_NOUGHT:
                // Servidor enviando jogada
                m.setBoardPosition(position, NOUGHT);
                break;
            
            case (char) PacketType::RECEIVE_WINNER:
                // Servidor enviando vencedor da partida
                m.printWinnerMessage(currentPacket.data1);
                break;
        }
    }

    close(clientSocket);

    return 0;
}