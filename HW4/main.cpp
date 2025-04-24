#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INDEX2D(row, col, nodes) (row * nodes + col)

void print_graph(size_t* graph, size_t nnodes) {
    for (size_t i = 0; i < nnodes; i++) {
        for (size_t j = 0; j < nnodes; j++) {
            printf("%ld ", graph[INDEX2D(i, j, nnodes)]);
        }
        printf("\n");
    }
} 

int main() {
    size_t nnodes, nedges;

    // get nodes and edges
    if(scanf("%lu %lu", &nnodes, &nedges) < 2) {
        fprintf(stderr, "Invalid number of nodes or edges");
        return 1;
    }
    
    // make graph and set all to infinity
    size_t* graph = (size_t*) malloc(sizeof(size_t)*nnodes*nnodes);
    memset(graph, 0xFF, sizeof(size_t)*nnodes*nnodes);
    
    for (size_t i = 0; i < nedges; i++) {
        size_t src, end, weight;
        if (scanf("%lu %lu %lu", &src, &end, &weight) < 3) {
            fprintf(stderr, "Failed to read edge %lu/%lu.\n", i + 1, nedges);
            return 1;
        }

        if (src < 1 || end < 1) {
            fprintf(stderr, "Invalid nodes for edge\n");
            return 1;
        }

        src -= 1;
        end -= 1;

        graph[INDEX2D(src, end, nnodes)] = weight;
        graph[INDEX2D(end, src, nnodes)] = weight;
    }

    print_graph(graph, nnodes);
    free(graph);

}
