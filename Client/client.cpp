#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>

int main() {
    // Testing the classes...
    printf("Working...\n");

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(8000); // 
    server_address.sin_addr.s_addr = INADDR_ANY;

    // TO-DO Try connection with server

    return 0;
}