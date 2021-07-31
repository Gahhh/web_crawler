//
// Created by Yu Liang on 31/7/21.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
// define the structure of adjacent node
typedef struct adjacent_node {
    struct vertex_node *v_node; // point to a vertex node
    size_t weight; // set the weight of this node
    struct adjacent_node *next; // point to the next adjacent node
} adjacent_node; // AKA AN

// define the structure of vertex node as a double linked list
typedef struct vertex_node {
    string data; // the information that this vertex had
    struct vertex_node *next; // point to the next vertex
    struct vertex_node *prev; // point to the previous vertex
    struct adjacent_node *first; // point to the first adjacent node
} vertex_node; // AKA VN

// define the graph structure
typedef struct Graph_Repr {
    struct vertex_node *first; // point to the first vertex
    struct vertex_node *last; // point to the last vertex
    int nV; // record the total number of vertex
    int nE; // record the total number of edges
} Graph_Repr;

int main() {
    graph G = graph_create();

    graph_add_vertex(G, "a");
    graph_add_vertex(G, "b");
    graph_add_vertex(G, "c");
    graph_add_vertex(G, "d");
    graph_add_vertex(G, "e");
    graph_add_vertex(G, "f");
    graph_add_vertex(G, "g");
    graph_add_vertex(G, "h");
    graph_add_vertex(G, "i");
    graph_add_vertex(G, "j");
    graph_add_vertex(G, "k");
    graph_add_vertex(G, "l");
    graph_add_vertex(G, "m");
    graph_add_vertex(G, "n");

    printf("should be 1: %d\n", graph_has_vertex(G, "e"));
    printf("should be 0: %d\n", graph_has_vertex(G, "z"));

    graph_remove_vertex(G, "x");
    graph_remove_vertex(G, "n");

    printf("vertex count: %lu\n", graph_vertices_count(G));

    graph_add_edge(G, "a", "b", 1);
    graph_add_edge(G, "b", "c", 1);
    graph_add_edge(G, "d", "e", 1);
    graph_add_edge(G, "c", "b", 1);
    graph_add_edge(G, "b", "a", 1);
    graph_add_edge(G, "d", "e", 1);
    graph_add_edge(G, "b", "q", 1);
    graph_add_edge(G, "q", "a", 1);
    graph_add_edge(G, "d", "e", 1);
    graph_add_edge(G, "j", "k", 1);
    graph_add_edge(G, "l", "m", 1);
    graph_add_edge(G, "b", "i", 1);
    graph_add_edge(G, "b", "f", 1);
    graph_add_edge(G, "b", "h", 1);


    printf("should be 1: %d\n", graph_has_vertex(G, "a", "b"));

    printf("remove should be 1: %lu\n", graph_remove_edge(G, "a", "b"));

    graph_set_edge(G, "b", "c", 5);

    printf("remove should be 5: %lu\n", graph_remove_edge(G, "b", "c"));

    printf("should be edge count: %lu\n", graph_edges_count(G, "b"));

    return 0;
}