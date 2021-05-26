#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "match.hpp"

#define DEBUG 1

void *listener(int socket_server, struct sockaddr serverclient){
    
}

void *sender()

int main() {

    int status;
    socklen_t addr_size;
    pid_t childproccess;
    char testBuffer[1024];
    char buffer[3];

    Match m;

    printf("Esperando conexões...\n");

    
    // Server socket address
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);    
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(8000); // 
    server_address.sin_addr.s_addr = INADDR_ANY;
    memset(&server_address.sin_zero, 0, sizeof(server_address.sin_zero));


    // Client sockets address
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in client_address;
    

    status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(DEBUG)
        if(status == -1) printf("Falha na função bind()...\n");

    status = listen(server_socket, 2);
    if(DEBUG)
        if(status == -1) printf("Falha na função listen()...\n");
    


    // Connection...
    while(1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &addr_size);
        
        // Check connection
        if(client_socket < 0)
            return 0;
        else
            printf("Conexão aceita: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        status = (childproccess = fork());

        if(status == 0) {
            close(server_socket);

            while(1) {
  
                recv(client_socket, testBuffer, 1024, 0);
  
                if(strcmp(buffer, ":exit") == 0){
                    printf("Desconectado de %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
                    break;
                }else {
  
                    printf("Client: %s\n", buffer);
                    send(client_socket, testBuffer, strlen(testBuffer), 0);
  
                    bzero(testBuffer, sizeof(testBuffer));
                }
            }
        }

    }

    close(client_socket);
    
    
    return 0;
}
