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

int dijkstra(size_t* graph, size_t src_node) {
    return 0;
}

int main() {
    size_t nnodes, nedges, src_index;

    // get nodes and edges
    if(scanf("%lu %lu", &nnodes, &nedges) < 2) {
        fprintf(stderr, "Invalid number of nodes or edges");
        return 1;
    }
    
    // make graph and set all to infinity
    size_t* graph = (size_t*) malloc(sizeof(size_t)*nnodes*nnodes);
    memset(graph, 0xFF, sizeof(size_t)*nnodes*nnodes);
    
    // read and populate graph
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

    // get source index
    if (scanf("%lu", &src_index) < 1) {
        fprintf(stderr, "Failed to get source node index\n");
        return 1;
    }
    if (src_index < 1) {
        fprintf(stderr, "Invalid source node index\n");
        return 1;
    } 
    
    // DEBUG
    print_graph(graph, nnodes);
    
    free(graph);
}
