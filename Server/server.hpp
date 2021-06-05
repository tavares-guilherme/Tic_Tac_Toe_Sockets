#ifndef __SERVER
#define __SERVER

#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "match.hpp"
#include "packet.hpp"

class Server {
    
    private:
        int serverSocket;
        std::mutex lock;
        Match currentMatch;

        bool serverIsOnline = false;
    
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

        bool online();
};


#endif
    