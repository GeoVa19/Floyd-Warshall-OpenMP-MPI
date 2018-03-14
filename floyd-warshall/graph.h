#ifndef _GRAPH
#define _GRAPH

#include <stdio.h>
#include <stdlib.h>

#define ERROR_MESSAGE // if this constant is defined, show_distances() will print an error message
//#undef ERROR_MESSAGE // undefining this constant, show_distances() won't print anything
#define WRITE_DISTANCES_TO_FILE // define this constant only when you want to write the output to a file (useful when NUMBER_OF_NODES is > 10)
#undef WRITE_DISTANCES_TO_FILE
#define NUMBER_OF_NODES 1000 //10, 100, 1000, 5000
#define MAX_NODES_TO_PRINT 15

// define error codes
#define NEGATIVE_NODE_NUMBER 0
#define OUT_OF_MEMORY 1
#define STEP_LESS_THAN_ONE 2

#define min(x,y) (((x) < (y)) ? (x) : (y))

// function prototypes
int **create_array(int number_of_nodes);
int **create_contiguous_array(int number_of_nodes);
void fill_array(int **dist);
int get_max_nodes_to_print();
void show_distances(int **dist);
void free_resources(int **dist);
void abort_with_error(int error_code);

#endif
