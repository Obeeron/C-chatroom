#ifndef COMMONH_H
#define COMMON_H

#define PORT 4242
#define BUFFER_SIZE 4096
#define MAX_MESSAGE_SIZE 2048
#define BACKLOG 4

typedef struct sockaddr SA;

void error(char *s);

#endif