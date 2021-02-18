#include <pthread.h>
#include "common.h"
    
int socket_fd = 0;
char username[MAX_NAME_SIZE];
int end = 0;

void get_username()
{
    char buffer[BUFFER_SIZE];
    do{
        printf("Username : ");
        //fflush(stdout);
        if(!fgets(buffer, BUFFER_SIZE, stdin)){
            error("fgets error while getting username.");
        }
        str_cut_nl(buffer, BUFFER_SIZE);
        if(is_name_valid(buffer))
            break;
        printf("Invalid username, length must be between %d and %d.\n",MIN_NAME_SIZE,MAX_NAME_SIZE);
    }while(1);
    strcpy(username,buffer);
}

void ctrl_c(int unused){
    unused = unused; // for unused argument warning
    end = 1;
}

int send_to_server(char str[]){
    return write(socket_fd, str, strlen(str)) < 0;
}

void new_client_line(){
    printf("\r > ");
    fflush(stdout);
}

void *message_in_handler(void* unused){
    unused = unused; // for unused argument warning
    char buffer[BUFFER_SIZE];
    while(!end){
        memset(buffer, 0, BUFFER_SIZE);
        if(read(socket_fd, buffer, BUFFER_SIZE) < 0){
            printf("Could not fetch last message");
        }
        printf("\n%s\n",buffer);
        new_client_line();
    }
    return NULL;
}

void *message_out_handler(void* unused){
    char buffer[BUFFER_SIZE];
    unused = unused; // for unused argument warning
    while(!end){
        new_client_line();
        if(!fgets(buffer, BUFFER_SIZE, stdin)){
            printf("Error with fgets.");
        }
        str_cut_nl(buffer, BUFFER_SIZE);
        if(!strcmp(buffer,"/exit") || !strcmp(buffer,"/leave")){
            end = 1;
            break;
        }
        send_to_server(buffer);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int port;
    pthread_t in_handler_tid, out_handler_tid;
    struct sockaddr_in server_addr;
    

    // Check for arguments
    if(argc < 3 || !(port = strtol(argv[2],NULL,10))){
        error("usage : %s <serverIpAdress> <port>");
    }

    // Get client username
    get_username();

    // Socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        error("Error while creating the socket");
    }
    
    // Server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    //server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if((inet_pton(AF_INET, argv[1], &server_addr.sin_addr)) <= 0){
        printf("Error on inet_pton for server address: %s\n",argv[1]);
        exit(1);
    }

    // Connect
    printf("Connecting to %s:%d..\n",argv[1],port);
    if(connect(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0)
        error("Could not establish a connexion!");
    printf("Connexion established to %s:%d !\n\n",argv[1],port);
    printf("--- Chatroom Client ---\n");
    
    // Send name
    if(send_to_server(username) < 0){
        error("Error sending name\n");
    }


    // // Receive
    // memset(buffer, 0, BUFFER_SIZE);
    // if(read(socket_fd, buffer, BUFFER_SIZE) >= 0){
    //     printf("%s",buffer);
    // }
    // else{
    //     error("\nCould not read the data from the server.");
    // }

    // Thread for handling incoming messages and sending messages
    pthread_create(&in_handler_tid, NULL, message_in_handler, NULL);
    pthread_create(&out_handler_tid, NULL, message_out_handler, NULL);

    // Redirecting CTRL+C signal
    signal(SIGINT, ctrl_c);

    // Waiting for client to exit
    while(!end){
        sleep(1);
    }

    close(socket_fd);
    printf("Connexion closed\n");
    return 0;
}