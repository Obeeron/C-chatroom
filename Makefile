CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2
LDFLAGS =
SERVER = server
CLIENT = client

SRC = server.c client.c common.c
OBJ = ${SRC:.c=.o} 

all: ${SRC}
	${CC} ${CFLAGS} server.c common.c -o ${SERVER}.out
	${CC} ${CFLAGS} client.c common.c -o ${CLIENT}.out

.PHONY: clean

clean:
	@rm ${SERVER}.out ${CLIENT}.out 2> /dev/null