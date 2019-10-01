CC=mpicc
INCDIR=../libgraph/include
LIBDIR=../libgraph/lib
#OBJ=
OPT=-O3 -march=native -mtune=native

.PHONY: all, clean

all: mpi_1 mpi_2 mpi_3

mpi_1.o:
	$(CC) -c mpi_first_version.c -I$(INCDIR) $(OPT)

mpi_2.o:
	$(CC) -c mpi_second_version.c -I$(INCDIR) $(OPT)

mpi_3.o:
	$(CC) -c mpi_third_version.c -I$(INCDIR) $(OPT)

mpi_1: mpi_1.o
	$(CC) -o mpi_1 mpi_first_version.c -I$(INCDIR) $(OPT) -L$(LIBDIR) -lgraph

mpi_2: mpi_2.o
	$(CC) -o mpi_2 mpi_second_version.c -I$(INCDIR) $(OPT) -L$(LIBDIR) -lgraph

mpi_3: mpi_3.o
	$(CC) -o mpi_3 mpi_third_version.c -I$(INCDIR) $(OPT) -L$(LIBDIR) -lgraph

clean:
	rm -f *.o mpi_1 mpi_2 mpi_3
