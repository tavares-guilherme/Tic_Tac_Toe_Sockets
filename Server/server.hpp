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

struct PlayerSocket {
    int socket;
    std::thread thread;
    struct sockaddr_in address;
};

class Server {
    private:
        int serverSocket;
        std::mutex lock;
        std::thread serverThread;

        std::vector<PlayerSocket> players;

        struct sockaddr_in server_address;
        Match currentMatch;
        
    public:
        Server();
        void registerNewConnection(int clientSocket, struct sockaddr_in address);
        void listener();
        void playerListener(int clientSocket, struct sockaddr_in address);
};


#endif
    