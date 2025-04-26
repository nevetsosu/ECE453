#include <stdio.h>
#include <vector>
#include <set>
#include <map>
#include <optional>

#define INDEX2D(row, col, nodes) (row * nodes + col)

typedef size_t Dist;
typedef size_t Node;
typedef size_t Edge;
typedef std::vector<std::optional<Dist>> Graph;
typedef std::pair<Dist, Node> DistTableEntry;
typedef std::map<Node, std::optional<DistTableEntry>> DistTable;

//
// DEBUG FUNCTIONS
//
/*
void print_table(DistTable const& distances) {
    for (auto itr = distances.cbegin(); itr != distances.cend(); itr++) {
        Node node = itr->first;
        std::optional<DistTableEntry> dist_entry = itr->second;
        if (dist_entry.has_value()) {
            printf("table index %lu: %ld, %ld\n", node, dist_entry.value().first, dist_entry.value().second);
        }
        else {
            printf("table index %lu: INF\n", node);
        }
    } 
}

void print_graph(Graph const& graph, Node nnodes) {
    for (Node i = 0; i < nnodes; i++) {
        for (Node j = 0; j < nnodes; j++) {
            printf("%3ld ", graph[INDEX2D(i, j, nnodes)].value_or(0));
        }
        printf("\n");
    }
}
*/

Graph BackTrace(DistTable const& distances, Node target, Node src) {
    if (!distances.at(target).has_value()) return Graph();
    Graph trace;
    
    Node node = target;
    trace.push_back(target);
    while (node != src) {
        node = distances.at(node).value().second;
        trace.push_back(node);
    }

    return trace;
}

void print_final(DistTable const& distances, Node src_node) {
    for (auto itr = distances.cbegin(); itr != distances.cend(); itr++) {
        if (!itr->second.has_value()) {
            printf("%lu: Distance = INF\n", itr->first + 1);
            continue;
        } 

        Dist dist = itr->second.value().first;
        printf("%lu: Distance = %lu, Path = ", itr->first + 1, dist);
        
        Graph trace = BackTrace(distances, itr->first, src_node);
        auto itr2 = trace.crbegin();
        printf("%lu", (*(itr2++)).value() + 1); 
        for (; itr2 != trace.crend(); itr2++) {
            printf(" -> %lu", (*itr2).value() + 1);
        }
        printf("\n");

    }
}

DistTable dijkstra(Graph graph, size_t nnodes, Node src_node) {
    std::set<Node> visited;
    DistTable distances;     // pairs are: distance, source_node, nullopts are INF
    
    // Picks new minimum distance that isn't in N'
    auto get_min = [&distances, &visited] () {
        std::optional<Node> min = std::nullopt;
        for (auto itr = distances.cbegin(); itr != distances.cend(); itr++) {
            if (visited.find(itr->first) != visited.cend() || !itr->second) continue;

            if (!min.has_value()) min = itr->first;
            else if (itr->second.value().first < distances.at(min.value()).value().first) {
                min = std::make_optional(itr->first); 
            }
        }

        return min;
    };

    // Initialize Distance Table
    visited.insert(src_node);
    for (Node i = 0; i < nnodes; i++) {
        if (i == src_node) continue;

        std::optional<Edge> edge = graph[INDEX2D(src_node, i, nnodes)];
        distances[i] = edge ? std::make_optional(std::make_pair(edge.value(), src_node)) : std::nullopt;
    }

    // main loop
    std::optional<Node> min_node;
    while ((min_node = get_min()).has_value()) {
        Dist min_node_dist = distances[min_node.value()].value().first;

        visited.insert(min_node.value());
        for (Node i = 0; i < nnodes; i++) {
            if (i ==  src_node) continue;

            std::optional<Edge> edge = graph[INDEX2D(min_node.value(), i, nnodes)];
            if (i == min_node || !edge.has_value()) continue;
            Dist new_dist = edge.value() + min_node_dist;

            if (!distances[i].has_value() || distances[i].value().first > new_dist) distances[i] = std::make_pair(new_dist, min_node.value());
        }
    }
    
    distances[src_node] = std::make_pair(0lu, src_node);
    return distances;
}

int main() {
    size_t nnodes, nedges;
    Node src_node;

    // get nodes and edges
    if(scanf("%lu %lu", &nnodes, &nedges) < 2) {
        fprintf(stderr, "Invalid number of nodes or edges");
        return 1;
    }
    
    // make graph and set all to infinity
    Graph graph(nnodes*nnodes);
    
    // read and populate graph
    for (size_t i = 0; i < nedges; i++) {
        Node src, end;
        Edge weight;
        if (scanf("%lu %lu %lu", &src, &end, &weight) < 3) {
            fprintf(stderr, "Failed to read edge %lu/%lu.\n", i + 1, nedges);
            return 1;
        }

        if (src < 1 || end < 1) {
            fprintf(stderr, "Invalid nodes for edge\n");
            return 1;
        }
        
        // convert nodes to 0-index
        src -= 1;
        end -= 1;

        graph[INDEX2D(src, end, nnodes)] = weight;
        graph[INDEX2D(end, src, nnodes)] = weight;
    }

    // get source index
    if (scanf("%lu", &src_node) < 1) {
        fprintf(stderr, "Failed to get source node index\n");
        return 1;
    }
    if (src_node < 1) {
        fprintf(stderr, "Invalid source node index\n");
        return 1;
    }
    src_node -= 1;

    DistTable distances = dijkstra(graph, nnodes, src_node);
    print_final(distances, src_node);
}
