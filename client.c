#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

int main(int argc, char *argv[]){
    int socket_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    if(argc < 2){
        error("usage : %s <serverIpAdress>");
    }

    // Socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        error("Error while creating the socket");
    }
    
    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) <= 0){
        printf("Error on inet_pton for server address: %s\n",argv[1]);
        exit(1);
    }

    // Connect
    if(connect(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0)
        error("Could not establish a connexion!");
    printf("Connexion established to %s:%d !\n\n",argv[1],PORT);
    
    // Receive
    memset(buffer, 0, BUFFER_SIZE);
    if(read(socket_fd, buffer, BUFFER_SIZE) >= 0){
        printf("%s",buffer);
    }
    else{
        error("\nCould not read the data from the server.");
    }
    
    printf("Connexion closed\n");
    close(socket_fd);
    return 0;
}