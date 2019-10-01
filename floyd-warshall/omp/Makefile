CC=gcc
LIBS=-fopenmp
INCDIR=../libgraph/include
LIBDIR=../libgraph/lib
OBJ=serial.o graph.o parallel.o
OPT=-O3 -march=native -mtune=native

.PHONY: all, clean

all: serial parallel

serial.o:
	$(CC) -c serial.c -I$(INCDIR) $(OPT)

serial: serial.o
	$(CC) -o serial serial.c -I$(INCDIR) $(OPT) -L$(LIBDIR) -lgraph

parallel.o:
	$(CC) -c parallel.c -I$(INCDIR) $(OPT)

parallel: parallel.o
	$(CC) -o parallel parallel.c -I$(INCDIR) $(LIBS) $(OPT) -L$(LIBDIR) -lgraph

clean:
	$(RM) *.o serial parallel

