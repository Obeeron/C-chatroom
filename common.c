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

int is_name_valid(char *name){
    int len = strlen(name);
    fflush(stdout);
    return len >= MIN_NAME_SIZE && len <= MAX_NAME_SIZE;
}

void clear_buffer(){
    int c;
    printf("Clearing buffer\n");
    while((c = getchar()) != '\n' && c != EOF){
        printf("c:%c\n",c);
    }
}

void str_cut_nl(char *s, int len){
    for(int i = 0; i<len && *s != '\0'; i++, s++){
        if(*s=='\n'){
            *s = '\0';
            break;
        }
    }
}