#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include "packet.hpp"

void sendPacket(Packet packet, int server) {
    char buffer[3];
    
    buffer[0] = packet.type;
    buffer[1] = packet.data1;
    buffer[2] = packet.data2;

    send(server, buffer, sizeof(char) * 3, 0);
}

Packet receivePacket(char buffer[3]) {
    Packet packet;

    packet.type  = buffer[0];
    packet.data1 = buffer[1];
    packet.data2 = buffer[2];

    return packet;
}