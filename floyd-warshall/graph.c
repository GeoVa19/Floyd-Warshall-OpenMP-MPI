#include "graph.h"

/**
  This function creates an NxN array.
**/
int **create_array(int number_of_nodes) {
  if (number_of_nodes <= 0) {
    abort_with_error(NEGATIVE_NODE_NUMBER);
  }

  int **dist = (int **) malloc(number_of_nodes*sizeof(int *));
  if (dist == NULL) {
    abort_with_error(OUT_OF_MEMORY);
  }

  for (int i = 0; i < number_of_nodes; i++) {
    dist[i] = (int *) malloc(number_of_nodes*sizeof(int));
    if (dist[i] == NULL) {
      abort_with_error(OUT_OF_MEMORY);
    }
  }

  return dist;
}

/**
  This function creates an NxN array as a contiguous block of memory.

  It is useful for the MPI implementation.

  Source: https://stackoverflow.com/questions/32942134/2d-array-as-contiguous-block-of-memory
**/
int **create_contiguous_array(int number_of_nodes) {
  int *data = (int *) malloc(number_of_nodes*number_of_nodes*sizeof(int));
  int **dist = (int **) malloc(number_of_nodes*sizeof(int *));

  for (int i = 0; i < number_of_nodes; i++) {
    dist[i] = &(data[number_of_nodes*i]);
  }

  return dist;
}

/**
  This function initializes every element of the graph array with a pseudo-random number.
**/
void fill_array(int **dist) {
  srand(42);

  for (int i = 0; i < NUMBER_OF_NODES; i++) {
    for (int j = 0; j < NUMBER_OF_NODES; j++) {
      if (i == j) {
        dist[i][j] = 0;
      } else {
        dist[i][j] = rand()%100; // [0, 100 - 1]
      }
    }
  }
}

int get_max_nodes_to_print() {
  #ifdef WRITE_DISTANCES_TO_FILE
    return NUMBER_OF_NODES;
  #else
    return MAX_NODES_TO_PRINT;
  #endif
}

/**
  This function prints the adjacency matrix of the graph.
**/
void show_distances(int **dist) {
  int max_nodes_to_print = get_max_nodes_to_print();
  if (NUMBER_OF_NODES <= max_nodes_to_print) {
    int i, j;

    printf("  ");
    for (i = 0; i < NUMBER_OF_NODES; i++) {
      printf("%3s%d ", "N", i);
    }
    printf("\n");
    for (i = 0; i < NUMBER_OF_NODES; i++) {
      printf("N%d", i);
      for (j = 0; j < NUMBER_OF_NODES; j++) {
        printf("%5d", dist[i][j]);
      }
      printf("\n");
    }
  } else {
    #ifdef ERROR_MESSAGE
      fprintf(stderr, "%s\n", "The graph is too large to print the distances!");
    #endif
  }
}

void free_resources(int **dist) {
  for (int i = 0; i < NUMBER_OF_NODES; i++) {
    free(dist[i]);
  }
  free(dist);
}

/**
  This function prints an error message and terminates the program.
**/
void abort_with_error(int error_code) {
  if (error_code == NEGATIVE_NODE_NUMBER) {
    fprintf(stderr, "The number of nodes must be positive!\n");
  } else if (error_code == OUT_OF_MEMORY) {
    fprintf(stderr, "Not enough memory!\n");
  } else if (error_code == STEP_LESS_THAN_ONE) {
    fprintf(stderr, "Number of threads should not exceed number of nodes!\n");
  }

  abort();
}
