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
#include "packet.hpp"

using namespace std;


int main() {
    // Testing connection ideas...

    char testBuffer[1024]; // Delete
    Match m;
    char buffer[3];
    int status;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;
    // Socket address definitions
    client_address.sin_family = AF_INET;
    client_address.sin_port   = htons(8000);
    client_address.sin_addr.s_addr = INADDR_ANY;

    status = connect(client_socket, (struct sockaddr*)&client_address, sizeof(client_address));

    if (status < 0) {
        cout << "Falha ao conectar-se com o servidor. Status: " << status << endl;

        return 1;
    }
    cout << "Conectado com sucesso." << endl;

    while(1) {
        Packet currentPacket;

        if (recv(client_socket, buffer, sizeof(char) * 3, 0) != -1) {
            currentPacket.type  = buffer[0];
            currentPacket.data1 = buffer[1]; // y
            currentPacket.data2 = buffer[2]; // x

            Position position;
            position.x = (int) currentPacket.data1;
            position.y = (int) currentPacket.data2;
            

            if (currentPacket.type == (char) PacketType::RECEIVE_NEW_MATCH) {
                // ??    
            } else if (currentPacket.type == (char) PacketType::RECEIVE_POSITION_CROSS) {
                m.setBoardPosition(position, CROSS);
            } else if (currentPacket.type == (char) PacketType::RECEIVE_POSITION_NOUGHT) {
                m.setBoardPosition(position, NOUGHT);
            } else if (currentPacket.type == (char) PacketType::RECEIVE_WINNER) {
                m.winnerMessage(currentPacket.data1);
            } else if (currentPacket.type == (char) PacketType::ASK_POSITION) {
                // pede a posição e manda pro server
                position = m.makePlay();

                sendPacket((char) PacketType::SEND_POSITION, position.x, position.y, client_socket);
            } else {
                cout << "Erro ao receber dados so servidor." << endl;
            }
        }
    }

    return 0;
}