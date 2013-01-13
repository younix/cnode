CC=gcc
CFLAGS=-std=c99 -Wall

.PHONY: all clean
all: cnode

cnode: cnode.c
	${CC} ${CFLAGS} -o $@ cnode.c

clean:
	rm -f cnode
