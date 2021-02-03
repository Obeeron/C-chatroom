#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 1234
#define MAX_SIZE 4096
#define BACKLOG 1
typedef struct sockaddr SA;

void error(char *s);

int main(){
    int socket_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;

    //TCP socket over IPV4
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0)
        error("Error while creating the socket");
    
    //Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    //Bind to address
    if(bind(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0)
        error("Error while binding the socket");

    //Put socket into listening mode, initializes backlog queue with size BACKLOG
    if(listen(socket_fd, BACKLOG) < 0)
        error("Can't put the socket into listening mode");

    socklen_t ca_size = sizeof(client_addr);
    char addr[1000];

    //Waiting for SYN from client
    printf("Waiting for connexion..\n\n");
    client_fd = accept(socket_fd, (SA*)&client_addr, &ca_size);
    
    inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, addr, 1000);
    printf("Connexion established to %s:%d !\n\n",addr,client_addr.sin_port);

    char buffer[MAX_SIZE+1];
    sprintf(buffer,"Server: Hello %s\n",addr);
    printf("%s",buffer);
    int bytes_to_send = strlen(buffer);
    
    if(write(client_fd,buffer,bytes_to_send) != bytes_to_send)
        error("Could not send the data to the client.");
    
    close(socket_fd);
    printf("Connexion closed\n");
    return 0;
}

void error(char *s){
    printf("%s\n\n",s);
    exit(1);
}