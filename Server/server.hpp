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

struct PlayerSocket {
    int socket;
    struct sockaddr_in *address; // Precisa ser passado como ponteiro
    std::thread thread;
};

class Server {

    
    private:
        int serverSocket;
        std::mutex lock;
    
    public:

        std::thread serverThread;
        std::vector<PlayerSocket> players;

        struct sockaddr_in *server_address;
        Match currentMatch;

        Server();
        void waitForConnection();


        void playerListener(int clientSocket, struct sockaddr_in address);        
        void registerNewConnection(int clientSocket, struct sockaddr_in address);

};


#endif
    