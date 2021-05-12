#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 1

int main() {

/*
    TO-DO:
        Establish connectin with two clients
        Invoke threads
 */

    printf("Waiting conection...\n");

    int status;
    int client_socket;
    char connection_message[50] = "Conectado com sucesso ao servidor.\n";

    // Initializing socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    // Defining server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port   = htons(8000); // 
    server_address.sin_addr.s_addr = INADDR_ANY;
    memset(&server_address.sin_zero, 0, sizeof(server_address.sin_zero));


    status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(DEBUG)
        if(status == -1) printf("Falha na função bind()...\n");

    status = listen(server_socket, 2);
    if(DEBUG)
        if(status == -1) printf("Falha na função listen()...\n");

    client_socket = accept(server_socket, 0, 0);

    send(client_socket, connection_message, sizeof(connection_message), 0);

    close(server_socket);
    
    return 0;
}