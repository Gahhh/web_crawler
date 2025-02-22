//
// Created by Yu Liang on 29/7/21.
//

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "list.h"
#include "graph.h"
#include "pagerank.h"
#include "dijkstra.h"

#define MAX_VALUE 2147483647

// define the structure of adjacent node
typedef struct Adjacent_Node {
    struct Vertex_Node *v_node; // point to a vertex node
    size_t weight; // set the weight of this node
    struct Adjacent_Node *next; // point to the next adjacent node
} Adjacent_Node;

// define the structure of vertex node as a double linked list
typedef struct Vertex_Node {
    string data; // the information that this vertex had
    double oldrank; // old rank of the page
    double pagerank; // current rank of the page;
    size_t D; // the number of outbound edges of vertex;
    struct Vertex_Node *next; // point to the next vertex
    struct Vertex_Node *prev; // point to the previous vertex
    struct Adjacent_Node *first; // point to the first adjacent node
    struct Adjacent_Node *inbound_first; // record the vertices which have edges point to this vertex
    struct Vertex_Node *pred; // used for Dijkstra
    size_t dist; // used for Dijkstra
    bool source; // used for Dijkstra to identify the source node
    bool visited; // used for Dijkstra
} Vertex_Node;

// define the graph structure
typedef struct Graph_Repr {
    struct Vertex_Node *first; // point to the first vertex
    struct Vertex_Node *last; // point to the last vertex
    int nV; // record the total number of vertex
    int nE; // record the total number of edges
} Graph_Repr;
// ================================Struct and utility functions for Dijkstra============================================
typedef struct List_Repr {
    struct Node *head;
    size_t length;
    struct Node *tail;
} List_Repr;

typedef struct Node {
    struct Vertex_Node *node_vertex;
    struct Node *next;
    struct Node *prev;
} Node;

void graph_dijkstra_enqueue (list ddlist, Vertex_Node *value) {
    Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->prev = NULL;
    new->node_vertex = value;
    if (!ddlist->head) {
        ddlist->tail = ddlist->head = new;
    } else {
        new->next = ddlist->head;
        ddlist->head->prev = new;
        ddlist->head = new;
    }
    ddlist->length++;
}

Vertex_Node* graph_dijkstra_dequeue (list ddlist) {
    if (ddlist->tail) {
        Vertex_Node *data = ddlist->tail->node_vertex;
        if (ddlist->tail == ddlist->head) {
            free(ddlist->tail);
            ddlist->tail = ddlist->head = NULL;
            ddlist->length--;
            return data;
        } else {
            ddlist->tail = ddlist->tail->prev;
            free(ddlist->tail->next);
            ddlist->tail->next = NULL;
            ddlist->length--;
            return data;
        }
    } else {
        return NULL;
    }
}

void graph_dijkstra_destroy (list ddlist) {
    if (ddlist->head) {
        Node *no_cur = ddlist->head;
        while (no_cur) {
            Node *temp = no_cur;
            no_cur = no_cur->next;
            free(temp);
        }
        ddlist->length = 0;
    }
    free(ddlist);
}

// ===========================================utility functions=========================================================

// This function is used in page_rank calculation, which is to calculate if all the pagerank is accurately enough.
bool is_differ_accepted (graph G, double delta) {
    Vertex_Node *p = G->first;
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
void vertex_add_inbound_node (graph G, Vertex_Node *vertex1, Vertex_Node *vertex2) {
    Adjacent_Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->weight = 0;
    new->v_node = vertex1;
    if (vertex2->inbound_first) {
        Adjacent_Node *p = vertex2->inbound_first;
        while (p->next) {
            p = p->next;
        }
        p->next = new;
    } else {
        vertex2->inbound_first = new;
    }
}

void adjacent_remove_inbound_node (graph G, Adjacent_Node *adj, Vertex_Node *vertex1, Vertex_Node *vertex2) {
    if (adj->next) {
        while (adj->next) {
            if (strcmp(adj->next->v_node->data, vertex1->data) == 0) {
                Adjacent_Node *temp = adj->next;
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
void vertex_remove_inbound_node (graph G, Vertex_Node *vertex1, string v2_data) {
    if (!vertex1->first->next && strcmp(vertex1->first->v_node->data, v2_data) == 0) {
        Adjacent_Node *p = vertex1->first->v_node->inbound_first;
        Vertex_Node *vertex2 = vertex1->first->v_node;
        adjacent_remove_inbound_node(G, p, vertex1, vertex2);
    } else {
        Adjacent_Node *p = vertex1->first;
        while(p) {
            if (strcmp(p->v_node->data, v2_data) == 0) {
                Adjacent_Node *p2 = p->v_node->inbound_first;
                Vertex_Node *vertex2 = p->v_node;
                adjacent_remove_inbound_node(G, p2, vertex1, vertex2);
            } else {
                p = p->next;
            }
        }
    }
}

// This function is to add adjacent node to the vertex.
void vertex_add_Adjacent_Node (graph G, Vertex_Node *vertex1, string v2_data, Adjacent_Node *temp, size_t weight) {
    Adjacent_Node *new = malloc(sizeof(*new));
    new->next = NULL;
    new->weight = weight;
    G->nE++;
    vertex1->D++;
    if (!temp) {
        vertex1->first = new;
    } else {
        temp->next = new;
    }
    Vertex_Node *p2 = G->first;
    while(p2 && strcmp(p2->data, v2_data) != 0) {
        p2 = p2->next;
    }
    new->v_node = p2;
    vertex_add_inbound_node(G, vertex1, p2);
}

// This function is to help sort function to swap two vertices
void vertex_swap (graph G, Vertex_Node *vertex1, Vertex_Node *vertex2) {
    if (vertex1->next) {
        vertex1->next->prev = vertex2;
        vertex2->next = vertex1->next;
        vertex2->prev->next = vertex1;
        vertex1->next = vertex2;
        vertex1->prev = vertex2->prev;
        vertex2->prev = vertex1;
    } else {
        vertex2->next = NULL;
        vertex2->prev->next = vertex1;
        vertex1->next = vertex2;
        vertex1->prev = vertex2->prev;
        vertex2->prev = vertex1;
        G->last = vertex2;
    }

}

// This function is to sort the vertex list by insertion sort method
void graph_sorted(graph G, double epsilon) {
    if (!G) return;
    if (!G->first) return;

    Vertex_Node *p1 = G->first;
    while (p1) {
        Vertex_Node *p2 = p1->next;
        while(p2 && p2->prev) {
            if ((p2->pagerank - p2->prev->pagerank) > epsilon) {
                vertex_swap(G, p2, p2->prev);
                p2 = p2->prev;
            } else if (fabs(p2->pagerank - p2->prev->pagerank) < epsilon) {
                if (strcmp(p2->data, p2->prev->data) < 0) {
                    vertex_swap(G, p2, p2->prev);
                    p2 = p2->prev;
                } else {
                    p2 = p2->prev;
                }
            } else {
                p2 = p2->prev;
            }
        }
        p1 = p1->next;
    }
    if (G->first->prev) {
        while (G->first->prev) {
            G->first = G->first->prev;
        }
    }
}
//======================================================================================================================


//======================================meta interface==================================================================

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
        Vertex_Node *curr = G->first;
        while (curr) {
            Vertex_Node *temp = curr;
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
    if (!file) {
        file = stdout;
    }
    Vertex_Node *p = G->first;
    while (p) {
        fprintf(file, "%s\n", p->data);
        p = p->next;
    }
    p = G->first;
    while (p) {
        Adjacent_Node *temp = p->first;
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
        Vertex_Node *new = malloc(sizeof(*new));
        new->D = 0;
        new->oldrank = 0;
        new->pagerank = 0;
        new->visited = false;
        new->inbound_first = NULL;
        new->first = NULL;
        new->next =new->prev = NULL;
        new->data = strdup(vertex);
        new->pred = NULL;
        new->source = false;
        new->dist = MAX_VALUE;
        G->first = new;
        G->last = new;
        G->nV++;
    } else {
        Vertex_Node *p = G->first;
        while(p && strcmp(p->data, vertex) != 0) {
            p = p->next;
        }
        if (!p) {
            Vertex_Node *new = malloc(sizeof(*new));
            new->D = 0;
            new->oldrank = 0;
            new->pagerank = 0;
            new->pred = NULL;
            new->dist = MAX_VALUE;
            new->source = false;
            new->visited = false;
            new->inbound_first = NULL;
            new->first = NULL;
            new->next =new->prev = NULL;
            new->data = strdup(vertex);
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
        Vertex_Node *p = G->first;
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
        Vertex_Node *p = G->first;
        // This is to remove all the inbound nodes of which vertex have edges with.
        while (p) {
            Adjacent_Node *adj = p->inbound_first;
            if (adj->next) {
                while (adj->next) {
                    if (strcmp(adj->next->v_node->data, vertex) == 0) {
                        Adjacent_Node *temp = adj->next;
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
                Adjacent_Node *curr = p->first;
                Adjacent_Node *next = p->first->next;
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
                    Adjacent_Node *temp = p->first;
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
    Vertex_Node *p = G->first;
    while (p) {
        // vertex1 is located when the expression is true
        if (strcmp(p->data, vertex1) == 0) {
            Adjacent_Node *temp = p->first;
            if (!temp) {
                vertex_add_Adjacent_Node(G, p, vertex2, temp, weight);
                return;
            } else {
                // this is to find if the edge is existed.
                while (temp) {
                    if (strcmp(temp->v_node->data, vertex2) == 0) {
                        return;
                    } else {
                        // the edge is not existed, therefore, we add an adjacent node to this list
                        if (!temp->next) {
                            vertex_add_Adjacent_Node(G, p, vertex2, temp, weight);
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
        Vertex_Node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex1) == 0) {
                Adjacent_Node *temp = p->first;
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
        Vertex_Node *p = G->first;
        while (p) {
            if (p->first && strcmp(p->data, vertex1) == 0) {
                if (!p->first->next && strcmp(p->first->v_node->data, vertex2) == 0) {
                    Adjacent_Node *temp = p->first;
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
                Adjacent_Node *curr = p->first;
                Adjacent_Node *next = p->first->next;
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
        Vertex_Node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex1) == 0) {
                Adjacent_Node *temp = p->first;
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
        Vertex_Node *p = G->first;
        while (p) {
            if (p->first && strcmp(p->data, vertex1) == 0) {
                Adjacent_Node *temp = p->first;
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
        Vertex_Node *p = G->first;
        while (p) {
            if (strcmp(p->data, vertex) == 0) {
                if (!p->first) {
                    return 0;
                } else {
                    size_t count = 1;
                    Adjacent_Node *temp = p->first;
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
    Vertex_Node *p = G->first; // This temporary pointer is to initialise the rank value
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
        double sink_rank = 0;
        while (p) {
            if (!p->D) {
                sink_rank = sink_rank + (damping * (p->oldrank/N));
            }
            p = p->next;
        }
        p = G->first; // This temporary pointer is to update the pagerank by sink_rank
        while (p) {
            p->pagerank = sink_rank + ((1-damping)/N);
            Adjacent_Node *adj = p->inbound_first;
            while (adj) {
                double D = adj->v_node->D; // convert data type from size_t to double
                p->pagerank = p->pagerank + ((damping * adj->v_node->oldrank)/D);
                adj = adj->next;
            }
            p = p->next;
        }
    }
    graph_sorted(G, delta);
}

void graph_viewrank(graph G, FILE *file) {
    if (!file) {
        file = stdout;
    }
    Vertex_Node *p = G->first;
    while (p) {
        fprintf(file, "%s (%.3f)\n", p->data, p->pagerank);
        p = p->next;
    }
}

void graph_shortest_path(graph G, string source) {
    if (!G) return;
    if(!G->first->next) return;

    // Create a queue for the nodes
    list queue = list_create();
    Vertex_Node *p = G->first;

    // This is to find the position of source node, and mark it as source.
    while (p && strcmp(p->data, source) != 0) {
        p = p->next;
    }
    if (p) {
        p->source = true;
        p->dist = 0;
        Adjacent_Node *adj = p->first;

        // This while loop is to initialize and enqueue all the nodes which have been pointed from the source node
        while (adj) {
            graph_dijkstra_enqueue(queue, adj->v_node);
            adj->v_node->pred = p;
            adj->v_node->dist = 1;
            adj->v_node->visited = true;
            adj = adj->next;
        }

        // This is to start the Dijkstra algorithm, for these nodes have been visited, we will compare the path value,
        // for these nodes have not been visited, we will add it to the queue.
        // After the queue become empty, all nodes' dist and pred will be identified.
        while (queue->length) {
            Vertex_Node *vertex = graph_dijkstra_dequeue(queue);
            size_t curr_dist = vertex->dist;
            if (vertex->first) {
                Adjacent_Node *adj2 = vertex->first;
                while (adj2) {
                    if (!adj2->v_node->source && curr_dist < adj2->v_node->dist) {
                        adj2->v_node->dist = curr_dist++;
                        adj2->v_node->pred = vertex;
                    }
                    if (!adj2->v_node->visited) {
                        adj2->v_node->visited = true;
                        graph_dijkstra_enqueue(queue, adj2->v_node);
                    }
                    adj2 = adj2->next;
                }
            }
        }
    }
    graph_dijkstra_destroy(queue);
}

void graph_view_path(graph G, string destination) {
    if (!G) return;
    list stack = list_create();
    Vertex_Node *p = G->first;
    while (p && strcmp(destination, p->data) != 0) {
        p = p->next;
    }
    if (p) {
        list_push(stack, p->data);
        Vertex_Node *p2 = p->pred;
        while (!p2->source) {
            list_push(stack, p2->data);
            p2 = p2->pred;
        }
        if (p2->source) {
            list_push(stack, p2->data);
        }
    } else {
        return;
    }
    while (stack) {
        string result = list_pop(stack);
        if (stack->length >= 1) {
            printf("%s -> ", result);
        } else {
            printf("%s\n", result);
            list_destroy(stack);
            return;
        }
    }

}
