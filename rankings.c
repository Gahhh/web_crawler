/**
 *
 * Written by: Dylan Brotherston <d.brotherston@unsw.edu.au>
 * Date: 2020/07/04
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>

#include "graph.h"
#include "pagerank.h"

graph read_cache            (string);

int main(int argc, char **argv)
{
    double damping_factor = .85;
    double epsilon = 0.00001;

    switch (argc) {
        case 4: {
            char *endptr = NULL;
            epsilon = strtod(argv[3], &endptr);
            if (*endptr != '\0') {
                fprintf(stderr, "'%s' is not a floating point number\n", argv[3]);
                return EXIT_FAILURE;
            }
            __attribute__ ((fallthrough));
        }
        case 3: {
            char *endptr = NULL;
            damping_factor = strtod(argv[2], &endptr);
            if (*endptr != '\0') {
                fprintf(stderr, "'%s' is not a floating point number\n", argv[2]);
                return EXIT_FAILURE;
            }
            __attribute__ ((fallthrough));
        }
        case 2: {
            break;
        }
        default: {
            fprintf(stderr, "Usage: %s <url> [<damping factor>] [<epsilon>]\n", argv[0]);
            return EXIT_FAILURE;
        }
    }

    graph network = read_cache(argv[1]);
    printf("Graph vertices and edges:\n");
    graph_show(network, stdout);
    printf("\nGraph PageRank:\n");
    graph_pagerank(network, damping_factor, epsilon);
    graph_viewrank(network, stdout);
    graph_destroy (network);

    return EXIT_SUCCESS;
}

graph read_cache(string url)
{
    if (access(url, R_OK) != 0) return NULL;
    FILE *cache = fopen(url, "r");
    if (!cache) return NULL;

    graph network = graph_create();
    if (!network) return NULL;

    char input_buffer[BUFSIZ];
    while (fgets(input_buffer, BUFSIZ, cache)) {
        if (!strchr(input_buffer, '\n')) {
            fprintf(stderr, "Line to Long: aborting reading cache file.\n");
            return NULL;
        }
        // split the line into an array of tokens
        size_t  n_tok = 1;
        char **toks = malloc(n_tok * sizeof(*toks));
        toks[n_tok - 1] = strtok(input_buffer, " \t\n");
        char *t = NULL;
        while ((t = strtok(NULL, " \t\n"))) {
            n_tok++;
            toks = realloc(toks, n_tok * sizeof(*toks));
            toks[n_tok - 1] = t;
        }
        switch (n_tok) {
            case 1: {
                graph_add_vertex(network, toks[0]);
                break;
            }
            case 3: {
                char *endptr = NULL;
                size_t weight =  strtol(toks[2], &endptr, 10);
                if (*endptr != '\0') {
                    fprintf(stderr, "weight is not numeric.\n");
                    free(toks);
                    graph_destroy(network);
                    return NULL;
                } else {
                    graph_add_edge(network, toks[0], toks[1], weight);
                }
                break;
            }
            default: {
                fprintf(stderr, "Line has incorrect number of tokens.\n");
                free(toks);
                graph_destroy(network);
                return NULL;
            }
        }
        free(toks);
    }

    fclose(cache);
    return network;
}
