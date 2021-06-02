#ifndef __PLAYERSOCKET
#define __ PLAYERSOCKET

#include <thread>
#include <mutex>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

struct PlayerSocket {
    int socket;
    struct sockaddr_in address; // Precisa ser passado como ponteiro
    std::thread thread;
};  

#endif