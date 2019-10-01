CC=gcc
CFLAGS=-O3

.PHONY: all, clean

all: graph.o libgraph.a

graph.o: src/graph.c
	$(CC) -c src/graph.c -Iinclude/ -o obj/graph.o $(CFLAGS)

libgraph.a: src/graph.c include/graph.h
	ar rcs lib/libgraph.a obj/graph.o

clean:
	rm -f obj/*.o lib/*.a
