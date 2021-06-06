#ifndef __PACKET
#define __PACKET

#include <iostream>
#include <string.h>
#include "match.hpp"

enum class PacketType : char {
    SEND_POSITION           = '0',
    SEND_NEW_MATCH          = '1',
    SEND_WINNER             = '2',
    RECEIVE_POSITION_CROSS  = '3',
    RECEIVE_POSITION_NOUGHT = '4',
    RECEIVE_WINNER          = '5',
    RECEIVE_NEW_MATCH       = '6',
    ASK_POSITION            = '7',
    WAITING_FOR_OPPONENT    = '8'
};

struct Packet {
    char type;
    char data1;
    char data2;
};

void sendPacket(char type, char data1, char data2, int n);
Packet receivePacket(char buffer[3]);
void multicastPacket(char type, char data1, char data2, int* servers, int n);

#endif