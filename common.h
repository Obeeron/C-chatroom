#ifndef COMMONH_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 4096
#define MAX_CLIENT 2
#define MAX_MESSAGE_SIZE 2048

#define MIN_NAME_SIZE 3
#define MAX_NAME_SIZE 20

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void error(char *s);
int is_name_valid(char *name);
void clear_buffer();
void str_cut_nl(char *s, int len);

#endif