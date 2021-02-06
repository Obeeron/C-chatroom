#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

void error(char *s){
    printf("%s\n",s);
    exit(EXIT_FAILURE);
}