#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include "packet.hpp"

void sendPacket(char type, char data1, char data2, int server) {
    // Manda pro server
    char buffer[3];
    
    buffer[0] = type;
    buffer[1] = data1;
    buffer[2] = data2;

    send(server, buffer, sizeof(char) * 3, 0);
}

Packet receivePacket(char buffer[3]) {
    Packet packet;

    packet.type = buffer[0];
    packet.data1 = buffer[1];
    packet.data2 = buffer[2];

    return packet;
}

void multicastPacket(char type, char data1, char data2, int* servers, int n) {
    for (int i = 0; i < n; i++) {
        sendPacket(type, data1, data2, servers[i]);
    }
}