//
// Created by Yu Liang on 29/7/21.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "graph.h"

// define the structure of adjacent node
typedef struct adjacent_node {
    struct vertex_node *v_node; // point to a vertex node
    size_t weight; // set the weight of this node
    struct adjacent_node *next; // point to the next adjacent node
} ANode; // AKA AN

typedef struct ANode *ANode;

// define the structure of vertex node as a double linked list
typedef struct vertex_node {
    string data; // the information that this vertex had
    struct vertex_node *next; // point to the next vertex
    struct vertex_node *prev; // point to the previous vertex
    struct adjacent_node *first; // point to the first adjacent node
} VNode; // AKA VN

typedef struct VNode *VNode;

// define the graph structure
typedef struct Graph_Repr {
    struct vertex_node *first; // point to the first vertex
    struct vertex_node *last; // point to the last vertex
    int nV; // record the total number of vertex
    int nE; // record the total number of edges
} Graph_Repr;

// meta interface
/**
 * graph_create
 * allocate the required memory for a new graph
 * return a pointer to the new graph
 * return NULL on error
 */
graph graph_create (void) {
    graph g = malloc(sizeof(*g));
    g->first = g->last = NULL;
    g->nE = g->nV = 0;
    return g;
}
/**
 * graph_destroy
 * free all memory associated with a given graph
 */
void graph_destroy (graph G) {
    if (G->first = NULL) {
        free(G);
        return;
    } else {
        VNode *curr = G->first;
        VNode *next = NULL;
        while (next) {
            next = curr->next;
            free(curr->data);
            free(curr);
            curr = next;
        }
        free(G);
    }
}
/**
 * graph_show
 * print the contents of the graph to the given file
 * If the given file is NULL, print to stdout
 * The graph should be printed in the following format:
 *      vertex1
 *      vertex2
 *      vertex3
 *      ...
 *      vertexN
 *      vertex1 vertex2 weight
 *      vertex2 vertex4 weight
 *      vertexN vertex4 weight
 * Where the name name of each vertex is first printed
 * Then the directed edges between each vertex along with the edge weight is printed
 */
void graph_show (graph G, FILE *file) {
    // identified two cases, if file is available, fprintf to the file,
    // if the file does not exist, then directly print to stdout.
    if (file) {
        fp = fopen(file, "w");
        VNode *p = G->first;
        while (p) {
            fprintf(fp, "%s\n", p->data);
            p = p->next;
        }
        p = G->first;
        while (p) {
            ANode *temp = p->first;
            while(temp) {
                fprintf(fp, "%s %s %lu", p->data, temp->v_node->data, temp->weight);
            }
        }
        fclose(fp);
    } else {
        VNode *p = G->first;
        while (p) {
            printf("%s\n", p->data);
            p = p->next;
        }
        p = G->first;
        while (p) {
            ANode *temp = p->first;
            while(temp) {
                printf("%s %s %lu", p->data, temp->v_node->data, temp->weight);
            }
        }
    }
}
// vertex interface
/**
 * graph_add_vertex
 * Add a new vertex with a particular value to the graph
 * if a vertex with the same value already exists do not add a new vertex
 */
void graph_add_vertex (graph G, string vertex) {
    if (!G) return;
    if (!G->first) {
        VNode *new = malloc(sizeof(*new));
        new->first = NULL;
        new->next =new->prev = NULL;
        new->data = strdup(string);
        G->first = G->last = new;
        G->nV++;
    } else {
        VNode *p = G->first;
        while(p && strcmp(p->data, vertex) != 0) {
            p = p->next
        }
        if (!p) {
            VNode *new = malloc(sizeof(*new));
            new->data = strdup(string);
            new->next = NULL;
            new->first = NULL;
            G->last->next = new;
            new->prev = G->last;
        }

    }
}
/**
 * graph_has_vertex
 * return True if a vertex with a particular value exists in the graph, False otherwise
 * return False on error
 */
bool graph_has_vertex (graph G, string vertex);
/**
 * graph_remove_vertex
 * Remove a vertex with a particular value from the graph
 */
void graph_remove_vertex (graph G, string vertex);
/**
 * graph_vertices_count
 * return the number of vertices in the graph
 * return 0 on error
 */
size_t graph_vertices_count (graph G);

// edge interface
/**
 * graph_add_edge
 * Add a new edge between two vertices with a weight to the graph
 * if a edge between two vertices already exists do not add a new edge
 */
void graph_add_edge (graph G, string vertex1, string vertex2, size_t weight);
/**
 * graph_has_edge
 * return True if a edge between two vertices exists in the graph, False otherwise
 * return False on error
 */
bool graph_has_edge (graph G, string vertex1, string vertex2);
/**
 * graph_remove_vertex
 * Remove a edge between two vertices from the graph
 * return the weight of the edge
 * return 0 on error
 */
size_t graph_remove_edge (graph G, string vertex1, string vertex2);
/**
 * graph_set_edge
 * Update the weight of a edge between two vertices, if the edge doesn't exist do nothing
 */
void graph_set_edge (graph G, string vertex1, string vertex2, size_t weight);
/**
 * graph_get_edge
 * return the weight of the edge between two vertices
 * return 0 on error
 */
size_t graph_get_edge (graph G, string vertex1, string vertex2);
/**
 * graph_edges_count
 * return the number of outgoing edges from a particular vertex in the graph
 * return 0 on error
 */
size_t graph_edges_count (graph G, string vertex);