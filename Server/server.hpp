#ifndef __SERVER
#define __SERVER

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include "match.hpp"
#include "packet.hpp"

struct Player{

}

class Server {
    
    private:
        int serverSocket;
        std::mutex lock;
    
    public:

        std::thread                     serverThread;

        std::vector<int>                playerSockets;
        std::vector<struct sockaddr_in> playerAddresses;
        std::vector<std::thread>        playerThreads;

        struct sockaddr_in server_address;

        Server();
        
        void waitForConnection();
        void registerNewConnection(int clientSocket, struct sockaddr_in address);
        void playerListener(int clientSocket, struct sockaddr_in address);

};


#endif
    