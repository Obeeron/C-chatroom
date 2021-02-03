CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2
LDFLAGS =

SRC = server.c client.c
OBJ = ${SRC:.c=.o} 
S_OBJ = server.o
C_OBJ = client.o

all: ${SRC}
	@${CC} ${CFLAGS} server.c -o server
	@${CC} ${CFLAGS} client.c -o client

server: ${S_OBJ}

client: ${C_OBJ}

.PHONY: clean

clean:
	rm ${OBJ} server client