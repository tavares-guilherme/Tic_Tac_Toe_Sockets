#ifndef __PACKET
#define __PACKET

#include <iostream>
#include <string.h>

enum class PacketType : char {
    SEND_POSITION           = '0',
    SEND_NEW_MATCH          = '1',
    SEND_WINNER             = '2',
    RECEIVE_POSITION_CROSS  = '3',
    RECEIVE_POSITION_NOUGHT = '4',
    RECEIVE_WINNER          = '5',
    RECEIVE_NEW_MATCH       = '6',
    ASK_POSITION            = '7',
    //WAITING_FOR_OPPONENT    = '8'
};

struct Packet {
    char type; //um dos tipos especificados em PacketType
    char data1; //posição X ou o vencedor da partida
    char data2; //posição Y
};

/**
  * @brief Envia um pacote para um servidor
  * 
  * @param packet O pacote a ser enviado
  * @param server O servidor que deve receber o pacote
  */
void sendPacket(Packet packet, int server);
/**
  * @brief Empacota a mensagem 
  * 
  * @param buffer A mensagem a ser empacotada
  * 
  * @return O pacote
  */
Packet receivePacket(char buffer[3]);

#endif