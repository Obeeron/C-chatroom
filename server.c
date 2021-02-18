#include "common.h"

typedef struct {
    int fd;
    SA_IN addr;
    char ip[16];
    char name[MAX_NAME_SIZE];
} cli_t;

typedef struct cli_list cli_list_t;
struct cli_list {
    cli_t *client;
    cli_list_t *next;
};

// Global vars
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
cli_list_t cli_list;
_Atomic int nb_clients = 0;

void add_client_to_list(cli_t *client)
{
    pthread_mutex_lock(&mutex);
    cli_list_t *new_cli = malloc(sizeof(cli_list_t));;
    new_cli->client = client;
    new_cli->next = cli_list.next;
    cli_list.next = new_cli;
    pthread_mutex_unlock(&mutex);
}

void remove_client_from_list(int fd)
{
    if(cli_list.next == NULL) return;
    pthread_mutex_lock(&mutex);
    for(cli_list_t *l = &cli_list; l->next != NULL; l = l->next){
        if(l->next->client->fd == fd){
            cli_list_t *tmp = l->next;
            l->next = tmp->next;
            free(tmp->client);
            free(tmp);
            break;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void broadcast_message(char *message, int client_fd)
{
    for(cli_list_t *l = cli_list.next; l != NULL; l = l->next){
        if(l->client->fd != client_fd){
            if(write(l->client->fd, message, strlen(message)) < 0){
                printf("ERROR : Could not send message to %s.\n",l->client->name);
            }
        }
    }
}

void *handle_client(void *client)
{
    cli_t *cli = (cli_t*)client;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE+strlen(cli->name)+2];
    int recv_status, end = 0;

    // Get client name
    if(recv(cli->fd, cli->name, MAX_NAME_SIZE, 0) < 0 || !is_name_valid(cli->name)){
        close(cli->fd);
        printf("Could not receive name or name is invalid. Connexion closed.\n");
        end = 1;
    }
    else{
        nb_clients++;
        sprintf(buffer,"%s has joined.", cli->name);
        broadcast_message(buffer, cli->fd);
        printf("%s\n",buffer);
    }

    // Client loop
    while(!end){
        memset(buffer,0,BUFFER_SIZE);
        recv_status = read(cli->fd, buffer, BUFFER_SIZE);
        //str_cut_nl(buffer,BUFFER_SIZE);
        if(recv_status <= 0){
            sprintf(buffer,"%s has left.",cli->name);
            broadcast_message(buffer,cli->fd);
            printf("%s\n",buffer);
            end = 1;
        }
        else if(recv_status > 0){
            sprintf(message,"%s: %s",cli->name,buffer);
            broadcast_message(message, cli->fd);
            printf("%s\n",message);
        }
    }

    // End connexion
    close(cli->fd);
    remove_client_from_list(cli->fd);
    nb_clients--;
    return NULL;
}

int main(int argc, char *argv[])
{
    int port, socket_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t ca_size = sizeof(client_addr);
    char addr[16];
    pthread_t thread_id;

    if(argc < 2 || !(port = strtol(argv[1],NULL,10))){
        error("usage : %s <port>");
    }

    //Socket
    if((socket_fd = socket(AF_INET, SOCK_STREAM,0)) < 0){
        error("Error while creating the socket");
    }

    // Address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    // Bind
    if(bind(socket_fd, (SA*)&server_addr, sizeof(server_addr)) < 0){
        error("Error while binding the socket");
    }

    // Listen
    if(listen(socket_fd, MAX_CLIENT) < 0){
        error("Can't put the socket into listening mode");
    }

    printf("--- Chatroom Server ---\n");
    while(1){
        // Receive new connexion
        client_fd = accept(socket_fd, (SA*)&client_addr, &ca_size);
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, addr, 1000);
        
        // Check for MAX_CLIENT
        if(nb_clients == MAX_CLIENT){
            close(client_fd);
            printf("<%s> tried to connect but room is full. Connexion rejected.\n",addr);
            continue;
        }

        // Create new client
        cli_t *cli = malloc(sizeof(cli_t));
        cli->fd = client_fd;
        cli->addr = client_addr;
        strcpy(cli->ip, addr);
        add_client_to_list(cli);

        // Thread client
        pthread_create(&thread_id, NULL, handle_client, (void*)cli);
    }
    
    close(socket_fd);
    printf("Chatroom closed.\n");
    return 0;
}