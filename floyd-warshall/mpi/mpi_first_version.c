#include <graph.h>
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
    int **dist = create_contiguous_array(NUMBER_OF_NODES);

    double time_from, time_to, lapsed_time;

    if (rank == ROOT) {
        fill_array(dist);
        show_distances(dist);
        time_from = MPI_Wtime();
    }

    MPI_Bcast(*dist, NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, MPI_COMM_WORLD); // send the array to every process

    if (rank != ROOT) {
        for (int k = 0; k < NUMBER_OF_NODES; k++) {
            for(int i = 0; i < NUMBER_OF_NODES; i++) {
                for (int j = 0; j < NUMBER_OF_NODES; j++) {
                    if (i == j) continue;
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
        MPI_Send(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, ROOT, 1, MPI_COMM_WORLD);
    } else {
        for (int source = 1; source < size; source++) {
            MPI_Recv(&(dist[0][0]), NUMBER_OF_NODES*NUMBER_OF_NODES, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
        }

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
