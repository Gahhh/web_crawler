// written by Subham Anand

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "pagerank.h"
#include "readData.h"
#include "graph.h"
#define MAXSTRING 1000
//#include "readData.c"

void calculatePageRank(double d, double diffPR, int maxIteration)
{
    //printf("hello");
    //int nov = num();
    char** Values = GetCollection();
    //printf("nov ---->%d", nov);
    Graph g = GetGraph();
    //printf("after graph");
    double *probability, *PrOld;

    //int x = nov + 3;

    probability = malloc(MAXSTRING*sizeof(double));
    PrOld = malloc(MAXSTRING*sizeof(double));

    double diff = diffPR, prob;
    int i , j,k ,edgecounter , countIteration = 0 , *edgesarray;

    edgesarray = malloc(MAXSTRING*sizeof(int));

    double constant = (1.0-d)/g->nV;

    for (i = 0 ; i < g->nV ; i++)
    {
        probability[i] = 1.0/g->nV;
    }

    while (countIteration < maxIteration && diff >= diffPR)
    {
        countIteration++;
        diff = 0;
        for (i = 0 ; i < g->nV ; i++)
        {
            PrOld[i] = probability[i];
        }

        for (i = 0 ; i < g->nV ; i++)
        {
            prob = 0.0;
            for (j = 0 ; j < g->nV ; j++)
            {
                if (g->edges[j][i] == 1)
                {
                    edgecounter = 0;
                    for (k = 0 ; k < g->nV ; k++)
                    {
                        if (g->edges[j][k] == 1)
                        {
                            edgecounter++;
                        }
                    }
                    prob += ((PrOld[j])/edgecounter);
                }
            }
            probability[i] = (d*prob) + constant;
            diff += fabs(probability[i] - PrOld[i]);
        }
    }

    //	for (i = 0 ; i < g->nV ; i++)
    //	{
    //		printf("probability[%d] = %.7f\n", i , probability[i]);
    //	}

    for (i = 0 ; i < g->nV ; i++)
    {
        edgecounter = 0;
        for (j = 0 ; j < g->nV ; j++)
        {
            if (g->edges[i][j] == 1)
            {
                edgecounter++;
            }
        }
        edgesarray[i] = edgecounter;
    }


    FILE *fp1;
    //printf("before file opening");
    if((fp1 = fopen("pagerankList.txt", "w")) != NULL)
    {
        //		printf("Error!");
        //      	exit(1);
        //	}
        //perror("error"); exit(1);
        //fp1 = fopen("pagerankList.txt", "w+");
        //printf("after file opening");
        int n = 0;
        while (n < g->nV)
        {
            double t1 = probability[i];
            int t2 = 0;
            for(i = 0; i < g->nV; i++)
            {
                if (t1 < probability[i])
                {
                    t1 = probability[i];
                    t2 = i;
                }
            }
            fprintf(fp1, "%s, %d, %.7f\n", Values[t2], edgesarray[t2], probability[t2]);
            probability[t2] = -1;
            n++;
        }
        fclose(fp1);
        //printf("after flie closing");
    }
}

int main(int argc, char *argv[])
{
    //Graph a = GetGraph();
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int c = atoi(argv[3]);
    calculatePageRank(a, b, c);
    //printf("wapas ");
    //calculatePageRank(0.85, 0.00001, 1000);
    //showGraph(a);
    return 0;
}
