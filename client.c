#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_PORT 1234
#define MAX_SIZE 4096
typedef struct sockaddr SA;

void error(char *s);

int main(int argc, char *argv[]){
    if(argc < 2)
        error("usage : %s <serverIpAdress>");

    int socket_fd;
    struct sockaddr_in server_addr;

    //TCP socket over IPV4
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0)
        error("Error while creating the socket");
    
    //Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) <= 0){
        printf("Error on inet_pton for server address: %s\n",argv[1]);
        exit(1);
    }

    if(connect(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0)
        error("Could not establish a connexion!");
    printf("Connexion established to %s:%d !\n\n",argv[1],SERVER_PORT);
    
    char buffer[MAX_SIZE+1];
    memset(buffer, 0, MAX_SIZE+1);
    int bytes_read;
    while((bytes_read = read(socket_fd, buffer, MAX_SIZE)) > 0){
        printf("%s",buffer);
    }
    if(bytes_read < 0)
        error("\nCould not read the data from the server.");
    
    printf("Connexion closed\n");
    close(socket_fd);
    return 0;
}

void error(char *s){
    printf("%s\n\n",s);
    exit(1);
}