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
      fprintf(stderr, "The number of processes cannot be 1!\n");
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

  double time_from, time_to, elapsed_time;

  int i[1] = {0};

  if (rank == ROOT) {
    fill_array(dist);
    show_distances(dist);
    time_from = MPI_Wtime();
  }

  MPI_Bcast(*dist, NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, MPI_COMM_WORLD); // send the array to every process

  while(i[0] < 2) { // in order for this to work, this block must be executed twice from each process
    if (rank != ROOT) {
      int start = (rank - 1)*step;
      int stop = start + step;
      if (rank == size - 1) {
        stop = NUMBER_OF_NODES;
      }
      // Floyd-Warshall algorithm. Each process has its own k.
      for (int k = start; k < stop; k++) {
        for(int i = 0; i < NUMBER_OF_NODES; i++) {
          for (int j = 0; j < NUMBER_OF_NODES; j++) {
            if (i == j) continue;
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
          }
        }
      }
    }
    i[0]++;
    // MPI_Allreduce combines values from all processes and distributes the result back to all processes.
    // It selects the minimum value of each element which is useful for this problem.
    // MPI_IN_PLACE saves memory space, and it prevents a segmentation fault.
    MPI_Allreduce(MPI_IN_PLACE, *dist, NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
  }

  if (rank == ROOT) {
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
