#include <graph.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void serial_implementation(int **dist);

int main() {
    int **dist; // NxN adjacency matrix

    dist = create_array(NUMBER_OF_NODES);

#ifndef WRITE_DISTANCES_TO_FILE
    printf("%s%d\n", "Number of nodes: ", NUMBER_OF_NODES);
#endif

    serial_implementation(dist);

    free_resources(dist);

    return EXIT_SUCCESS;
}

/**
  This function implements Floyd-Warshall algorithm without any parallelization.
**/
void serial_implementation(int **dist) {
    time_t start, end;

    fill_array(dist);

    show_distances(dist);

    time(&start); // start measuring time
    for (int k = 0; k < NUMBER_OF_NODES; k++) {
        for (int i = 0; i < NUMBER_OF_NODES; i++) {
            for (int j = 0; j < NUMBER_OF_NODES; j++) {
                if (i == j) continue;
                dist[i][j] = MIN(dist[i][j], dist[i][k] + dist[k][j]);
        }
        }
    }
    time(&end); // end measuring time

    show_distances(dist);

#ifndef WRITE_DISTANCES_TO_FILE
    printf("Serial implementation: total elapsed time: %.6f sec.\n", difftime(end, start));
#endif
}
