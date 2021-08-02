//
// Created by Yu Liang on 29/7/21.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "pagerank.h"

// define the structure of adjacent node
typedef struct adjacent_node {
    struct vertex_node *v_node; // point to a vertex node
    size_t weight; // set the weight of this node
    struct adjacent_node *next; // point to the next adjacent node
} adjacent_node;

// define the structure of vertex node as a double linked list
typedef struct vertex_node {
    string data; // the information that this vertex had
    double oldrank; // old rank of the page
    double pagerank; // current rank of the page;
    double sink_rank; // for pagerank
    size_t D; // the number of outbound edges of vertex;
    struct vertex_node *next; // point to the next vertex
    struct vertex_node *prev; // point to the previous vertex
    struct adjacent_node *first; // point to the first adjacent node
    struct adjacent_node *inbound_first; // record the vertices which have edges point to this vertex
} vertex_node;

// define the graph structure
typedef struct Graph_Repr {
    struct vertex_node *first; // point to the first vertex
    struct vertex_node *last; // point to the last vertex
    int nV; // record the total number of vertex
    int nE; // record the total number of edges
} Graph_Repr;

// utility functions

// This function is used in page_rank calculation, which is to calculate if all the pagerank is accurately enough.
bool is_differ_accepted (graph G, double delta) {
    vertex_node *p = G->first;
    while(p) {
        if (fabs(p->oldrank - p->pagerank) > delta) {
            return false;
        } else {
            p = p->next;
        }
    }
    return true;
}

// this function is to record the inbound node for the vertex.
void vertex_add_inbound_node (graph G, vertex_node *vertex1, vertex_node *vertex2) {
    adjacent_node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->weight = 0;
    new->v_node = vertex1;
    if (vertex2->inbound_first) {
        adjacent_node *p = vertex2->inbound_first;
        while (p->next) {
            p = p->next;
        }
        p->next = new;
    } else {
        vertex2->inbound_first = new;
    }
}

void adjacent_remove_inbound_node (graph G, adjacent_node *adj, vertex_node *vertex1, vertex_node *vertex2) {
    if (adj->next) {
        while (adj->next) {
            if (strcmp(adj->next->v_node->data, vertex1->data) == 0) {
                adjacent_node *temp = adj->next;
                adj->next = adj->next->next;
                free(temp);
                return;
            } else {
                adj = adj->next;
            }
        }
    } else {
        adj = NULL;
        vertex2->inbound_first = NULL;
        free(adj);
    }
}

// This function is to remove inbound node when its relevant edge has been removed.
void vertex_remove_inbound_node (graph G, vertex_node *vertex1, string v2_data) {
    if (!vertex1->first->next && strcmp(vertex1->first->v_node->data, v2_data) == 0) {
        adjacent_node *p = vertex1->first->v_node->inbound_first;
        vertex_node *vertex2 = vertex1->first->v_node;
        adjacent_remove_inbound_node(G, p, vertex1, vertex2);
    } else {
        adjacent_node *p = vertex1->first;
        while(p) {
            if (strcmp(p->v_node->data, v2_data) == 0) {
                adjacent_node *p2 = p->v_node->inbound_first;
                vertex_node *vertex2 = p->v_node;
                adjacent_remove_inbound_node(G, p2, vertex1, vertex2);
            } else {
                p = p->next;
            }
        }
    }
}

// This function is to add adjacent node to the vertex.
void vertex_add_adjacent_node (graph G, vertex_node *vertex1, string v2_data, adjacent_node *temp, size_t weight) {
    adjacent_node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->weight = weight;
    G->nE++;
    vertex1->D++;
    if (!temp) {
        vertex1->first = new;
    } else {
        temp->next = new;
    }
    vertex_node *p2 = G->first;
    while(p2 && strcmp(p2->data, v2_data) != 0) {
        p2 = p2->next;
    }
    new->v_node = p2;
    vertex_add_inbound_node(G, vertex1, p2);
}



// meta interface
graph graph_create (void) {
    graph g = malloc(sizeof(*g));
    g->first = g->last = NULL;
    g->nE = g->nV = 0;
    return g;
}

void graph_destroy (graph G) {
    if (G->first == NULL) {
        free(G);
        return;
    } else {
        vertex_node *curr = G->first;
        while (curr) {
            vertex_node *temp = curr;
            curr = curr->next;
            free(temp->data);
            free(temp);
        }
        free(G);
    }
}

void graph_show (graph G, FILE *file) {
    // identified two cases, if file is available, fprintf to the file,
    // if the file does not exist, then directly print to stdout.
    vertex_node *p = G->first;
    while (p) {
        fprintf(file, "%s\n", p->data);
        p = p->next;
    }
    p = G->first;
    while (p) {
        adjacent_node *temp = p->first;
        while(temp) {
            fprintf(file, "%s %s %lu\n", p->data, temp->v_node->data, temp->weight);
            temp = temp->next;
        }
        p = p->next;
    }
}
// vertex interface

void graph_add_vertex (graph G, string vertex) {
    if (!G) return;
    if (!G->first) {
        vertex_node *new = malloc(sizeof(*new));
        new->D = new->oldrank = new->pagerank = new->sink_rank = 0;
        new->inbound_first = NULL;
        new->first = NULL;
        new->next =new->prev = NULL;
        new->data = strdup(vertex);
        G->first = G->last = new;
        G->nV++;
    } else {
        vertex_node *p = G->first;
        while(p && strcmp(p->data, vertex) != 0) {
            p = p->next;
        }
        if (!p) {
            vertex_node *new = malloc(sizeof(*new));
            new->D = new->oldrank = new->pagerank = new->sink_rank = 0;
            new->data = strdup(vertex);
            new->next = NULL;
            new->first = NULL;
            new->inbound_first = NULL;
            G->last->next = new;
            new->prev = G->last;
            G->last = new;
            G->nV++;
        }

    }
}

bool graph_has_vertex (graph G, string vertex) {
    if (!G) return false;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (strcmp(vertex, p->data) == 0) {
                return true;
            } else {
                p = p->next;
            }
        }
    } else {
        return false;
    }
    return false;
}

void graph_remove_vertex (graph G, string vertex) {
    if (!G) return;
    if (G->first) {
        vertex_node *p = G->first;
        // This is to remove all the inbound nodes of which vertex have edges with.
        while (p) {
            adjacent_node *adj = p->inbound_first;
            if (adj->next) {
                while (adj->next) {
                    if (strcmp(adj->next->v_node->data, vertex) == 0) {
                        adjacent_node *temp = adj->next;
                        adj->next = adj->next->next;
                        free(temp);
                    } else {
                        adj = adj->next;
                    }
                }
            } else {
                if (strcmp(adj->v_node->data, vertex) == 0) {
                    free(adj);
                    p->inbound_first = NULL;
                }
            }
            p = p->next;
        }
        p = G->first;
        while (p && strcmp(vertex, p->data) != 0) {
            p = p->next;
        }
        if (p && G->first != G->last) {
            if (p->first->next) {
                adjacent_node *curr = p->first;
                adjacent_node *next = p->first->next;
                while(next) {
                    if (strcmp(next->v_node->data, vertex) == 0) {
                        curr->next = next->next;
                        free(next);
                        next = curr->next;
                        G->nE--;
                    } else {
                        curr = next;
                        next = next->next;
                    }
                }
            } else {
                if (strcmp(p->first->v_node->data, vertex) == 0) {
                    adjacent_node *temp = p->first;
                    p->first = NULL;
                    free(temp);
                    G->nE--;
                }
            }
            p->next->prev = p->prev;
            p->prev->next = p->next;
            free(p);
            G->nV--;
        } else {
            G->first = G->last = NULL;
            G->nV--;
            free(p);
        }
    }
}

size_t graph_vertices_count (graph G) {
    if (G) {
        return G->nV;
    } else {
        return 0;
    }
}

// edge interface

void graph_add_edge (graph G, string vertex1, string vertex2, size_t weight) {
    if (!G) return;
    // These two 'if' expression below is to add the vertex to the graph if it does not exist in the graph
    if (!graph_has_vertex(G, vertex1)) graph_add_vertex(G, vertex1);
    if (!graph_has_vertex(G, vertex2)) graph_add_vertex(G, vertex2);
    // This temporary p is to find the location of vertex1
    vertex_node *p = G->first;
    while (p) {
        // vertex1 is located when the expression is true
        if (strcmp(p->data, vertex1) == 0) {
            adjacent_node *temp = p->first;
            if (!temp) {
                vertex_add_adjacent_node(G, p, vertex2, temp, weight);
                return;
            } else {
                // this is to find if the edge is existed.
                while (temp) {
                    if (strcmp(temp->v_node->data, vertex2) == 0) {
                        return;
                    } else {
                        // the edge is not existed, therefore, we add an adjacent node to this list
                        if (!temp->next) {
                            vertex_add_adjacent_node(G, p, vertex2, temp, weight);
                            return;
                        } else {
                            temp = temp->next;
                        }
                    }
                }
            }
        } else {
            p = p->next;
        }
    }
}

bool graph_has_edge (graph G, string vertex1, string vertex2) {
    if(!G) return false;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex1) == 0) {
                adjacent_node *temp = p->first;
                while (temp && strcmp(temp->v_node->data, vertex2) != 0) {
                    temp = temp->next;
                }
                if (temp) {
                    return true;
                } else {
                    return false;
                }
            } else {
                p = p->next;
            }
        }
    }
    return false;
}

size_t graph_remove_edge (graph G, string vertex1, string vertex2) {
    if (!G) return 0;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (p->first && strcmp(p->data, vertex1) == 0) {
                if (!p->first->next && strcmp(p->first->v_node->data, vertex2) == 0) {
                    adjacent_node *temp = p->first;
                    vertex_remove_inbound_node(G, p, vertex2);
                    p->first = NULL;
                    size_t data = temp->weight;
                    free(temp);
                    G->nE--;
                    p->D--;
                    return data;
                } else if (!p->first->next && strcmp(p->first->v_node->data, vertex2) != 0) {
                    return 0;
                }
                adjacent_node *curr = p->first;
                adjacent_node *next = p->first->next;
                while (next) {
                    if (strcmp(next->v_node->data, vertex2) == 0) {
                        curr->next = next->next;
                        size_t data = next->weight;
                        vertex_remove_inbound_node(G, p, vertex2);
                        free(next);
                        G->nE--;
                        p->D--;
                        return data;
                    } else {
                        curr = curr->next;
                        next = next->next;
                    }
                }
            } else {
                p = p->next;
            }
        }
    }
    return 0;
}

void graph_set_edge (graph G, string vertex1, string vertex2, size_t weight) {
    if (!G) return;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex1) == 0) {
                adjacent_node *temp = p->first;
                while (temp) {
                    if (strcmp(temp->v_node->data, vertex2) == 0) {
                        temp->weight = weight;
                        return;
                    } else {
                        temp = temp->next;
                    }
                }
                return;
            } else {
                p = p->next;
            }
        }
    }
}

size_t graph_get_edge (graph G, string vertex1, string vertex2) {
    if (!G) return 0;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (p->first && strcmp(p->data, vertex1) == 0) {
                adjacent_node *temp = p->first;
                while (temp && strcmp(temp->v_node->data, vertex2) != 0) {
                    temp = temp->next;
                }
                if (temp) {
                    return temp->weight;
                }
            } else {
                p = p->next;
            }
        }
    }
    return 0;
}

size_t graph_edges_count (graph G, string vertex) {
    if (!G) return 0;
    if (G->first) {
        vertex_node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex) == 0) {
                if (!p->first) {
                    return 0;
                } else {
                    size_t count = 1;
                    adjacent_node *temp = p->first;
                    while (temp) {
                        count++;
                        temp = temp->next;
                    }
                    return count;
                }
            } else {
                p = p->next;
            }
        }
    }
    return 0;
}

void graph_pagerank(graph G, double damping, double delta) {
    size_t N_vertices = graph_vertices_count(G);
    double N = N_vertices;
    if (N == 0) return;
    vertex_node *p = G->first; // This temporary pointer is to initialise the rank value
    while (p) {
        p->oldrank = 0;
        p->pagerank = 1/N;
        p = p->next;
    }
    while (!is_differ_accepted(G, delta)) {
        p = G->first; // This temporary pointer is to update the oldrank value with current one
        while (p) {
            p->oldrank = p->pagerank;
            p = p->next;
        }
        p = G->first; // This temporary pointer is to find vertices with no outbound edges.
        while (p) {
            if (!p->D) {
                p->sink_rank = p->sink_rank + (damping *(p->oldrank/N));
            }
            p = p->next;
        }
        p = G->first; // This temporary pointer is to update the pagerank by sink_rank
        while (p) {
            p->pagerank = p->sink_rank + ((1-damping)/N);
            adjacent_node *adj = p->inbound_first;
            while (adj) {
                double D = adj->v_node->D; // convert data type from size_t to double
                printf("edges from I: %lu, I: %s\n", adj->v_node->D, adj->v_node->data);
                adj->v_node->pagerank = p->pagerank + ((damping*adj->v_node->oldrank)/D);
                adj = adj->next;
            }
            p = p->next;
        }
    }
}

void graph_viewrank(graph G, FILE *file) {
    vertex_node *p = G->first;
    printf(" \n");
    while (p) {
        adjacent_node *temp = p->inbound_first;
        while(temp) {
            printf("inbound: from %s | to %s\n", temp->v_node->data, p->data);
            temp = temp->next;
        }
        p = p->next;
    }
    printf(" \n");
    p = G->first;
    while (p) {
        fprintf(file, "%s (%f)\n", p->data, p->pagerank);
        p = p->next;
    }
}
