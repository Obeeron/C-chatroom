#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

int main(){
    int socket_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t ca_size = sizeof(client_addr);
    char buffer[BUFFER_SIZE], addr[16];

    //Socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        error("Error while creating the socket");
    }

    // Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    // Bind
    if(bind(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0){
        error("Error while binding the socket");
    }

    // Listen
    if(listen(socket_fd, BACKLOG) < 0){
        error("Can't put the socket into listening mode");
    }
    printf("Waiting for connexion..\n\n");

    // Accept connexion
    client_fd = accept(socket_fd, (SA*)&client_addr, &ca_size);
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, addr, 1000);
    printf("Connexion established to %s:%d !\n\n",addr,client_addr.sin_port);

    // Send data
    sprintf(buffer,"Server: Hello %s\n",addr);
    printf("%s",buffer);
    int bytes_to_send = strlen(buffer);
    if(write(client_fd,buffer,bytes_to_send) != bytes_to_send){
        error("Could not send the data to the client.");
    }
    
    close(socket_fd);
    printf("Connexion closed\n");
    return 0;
}