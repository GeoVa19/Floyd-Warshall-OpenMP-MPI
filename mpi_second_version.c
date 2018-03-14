#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  MPI_Status status;
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size == 1) {
    fprintf(stderr, "Number of processes cannot be 1!\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  const int ROOT = 0;

  int step = NUMBER_OF_NODES / size;

  if (step < 1) {
    if (rank == ROOT) {
      fprintf(stderr, "The number of processes should not exceed the number of nodes!\n");
    }
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int **dist = create_contiguous_array(NUMBER_OF_NODES);

  double time_from, time_to, lapsed_time;

  if (rank != ROOT) {
    MPI_Recv(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, rank - 1, 1, MPI_COMM_WORLD, &status); // receive dist array from previous process
    int start = (rank - 1)*step;
    int stop = start + step;
    if (rank == size - 1) {
      stop = NUMBER_OF_NODES;
    }
    // Floyd-Warshall algorithm. Each process has its own k.
    for (int k = start; k < stop; k++) {
      for (int i = 0; i < NUMBER_OF_NODES; i++) {
        for (int j = 0; j < NUMBER_OF_NODES; j++) {
          if (i == j) continue;
          dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        }
      }
    }
  }

  if (rank < size - 1) {
    if (rank == ROOT) { // master process
      fill_array(dist); // fill array with pseudo-random numbers
      show_distances(dist);
      time_from = MPI_Wtime();
    }
    MPI_Send(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, rank + 1, 1, MPI_COMM_WORLD); // send dist array to the next process
  }

  if (rank == size - 1) {
    MPI_Send(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, 1, MPI_COMM_WORLD); // the last process sends the dist array back to master process
  }

  if (rank == ROOT) {
    MPI_Recv(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, size - 1, 1, MPI_COMM_WORLD, &status); // get the dist array from the last process
    double time_to = MPI_Wtime();
    show_distances(dist);

    double lapsed_time = time_to - time_from;
    #ifndef WRITE_DISTANCES_TO_FILE
      printf("Elapsed time: %.6f seconds.\n", lapsed_time);
    #endif
  }

  free(dist);

  MPI_Finalize();
}
