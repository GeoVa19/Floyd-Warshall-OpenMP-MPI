#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define NUMBER_OF_THREADS 6

void mp_implementation(int **dist);

int main() {
    int **dist;

    dist = create_array(NUMBER_OF_NODES);

#ifndef WRITE_DISTANCES_TO_FILE
    printf("%s%d\n", "Number of nodes: ", NUMBER_OF_NODES);
#endif

    mp_implementation(dist);

    free_resources(dist);

    return EXIT_SUCCESS;
}

/**
  This function implements Floyd-Warshall algorithm
  using the OpenMP API.
**/
void mp_implementation(int **dist) {
    double time_start, time_end;

    fill_array(dist);

    show_distances(dist);

    int thread_id;
    int step = NUMBER_OF_NODES/NUMBER_OF_THREADS, start, stop;
    int i, j, k;

    if (step < 1) {
        /** the program aborts when the number of threads is greater than the number of nodes
            (since the last thread will do all the work by itself) **/
        abort_with_error(STEP_LESS_THAN_ONE);
    }

    omp_set_num_threads(NUMBER_OF_THREADS); // set the number of threads

    time_start = omp_get_wtime(); // start measuring time
    // each thread must have its own thread id, start and stop indices of the dist array
    #pragma omp parallel private(thread_id, start, stop)
    {
        thread_id = omp_get_thread_num();
        start = step*thread_id; // set start and stop indices for variable k
        stop = start + step;

        if (thread_id == omp_get_num_threads() - 1) {
        stop = NUMBER_OF_NODES;
        }

        // run Floyd-Warshall algorithm using threads
        #pragma omp parallel for private(i, j)
        for (k = start; k < stop; k++) {
            for (i = 0; i < NUMBER_OF_NODES; i++) {
                for (j = 0; j < NUMBER_OF_NODES; j++) {
                    if (i == j) continue;
                    dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    time_end = omp_get_wtime(); // end measuring time

    double elapsed_time = time_end - time_start;

    show_distances(dist);

#ifndef WRITE_DISTANCES_TO_FILE
    printf("OpenMP: total elapsed time: %.6f sec. Number of threads: %d.\n", elapsed_time, NUMBER_OF_THREADS);
#endif
}
