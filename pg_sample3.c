//
// Created by Yu Liang on 1/8/21.
//

//
// Created by HUGO on 22/1/20.
//

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <assert.h>
# include "DLListStr.h"
# include "readdata.h"
# include "graph.h"
# include <math.h>

# define MAX_STRING 1000

void pagerank(double division, double diffPR, int maxIteration, double *vector, int *urlsDegree, Graph g, int numberofurl,
              int *urlIndex) {

    // set 2d matrix and put the original data
    double **matrix = malloc(numberofurl * sizeof(double));
    for (int i = 0; i < numberofurl; i++) {
        // get every degree of each vertex
        int numberofDegree = getNumberOfOutDegrees(g, i);

        matrix[i] = malloc(numberofurl * sizeof(double));
        for (int j = 0; j < numberofurl; j++) {
            if (adjacent(g, i, j)) {
                matrix[i][j] = 1.0 / numberofDegree;
            } else {
                // when outdegree is zero
                for (int j = 0; j < numberofurl; j++) {
                    matrix[j][i] = 1.0 / numberofurl;
                }
            }
        }
        urlsDegree[i] = numberofDegree;
    }

    // reverse the matrix
    double **reversematrix = malloc(numberofurl * sizeof(double));
    for (int i = 0; i < numberofurl; i++) {
        reversematrix[i] = malloc(numberofurl * sizeof(double));
        for (int j = 0; j < numberofurl; j++) {
            reversematrix[i][j] = matrix[j][i];
        }
    }


    // init set
    int iteration = 0;
    double diff = diffPR;
    double *curMatrix = malloc(numberofurl * sizeof(double));

    // iteration
    while (iteration < maxIteration && diff >= diffPR) {
        for (int i = 0; i < numberofurl; i++) {
            double sum = 0.0;
            for (int j = 0; j < numberofurl; j++) {
                sum += vector[j] * reversematrix[i][j];
            }
            //printf("total is %f\n",sum);
            // compute PR
            curMatrix[i] = (1.0 - division) / numberofurl + division * sum;
            //printf("curMatrix[%d] = %f\n",i,curMatrix[i]);

        }

        // compute diff
        diff = 0.0;
        for (int i = 0; i < numberofurl; i++) {
            diff = diff + fabs(curMatrix[i] - vector[i]);
            vector[i] = curMatrix[i];
        }
        iteration++;
    }

    // sorted
    for (int i = 0; i < numberofurl; i++) {
        int index = urlIndex[i];
        int j = i - 1;
        int sign = 0;
        if (vector[j] == vector[i]) {
            sign = urlsDegree[j] < urlsDegree[i];
        } else {
            sign = vector[j] < vector[i];
        }
        while (j >= -1 && sign >0) {
            if (vector[j] == vector[i]) {
                sign = urlsDegree[j] < urlsDegree[i];
            } else {
                sign = vector[j] < vector[i];
            }
            urlIndex[j + 1] = urlIndex[j];
            j--;
        }
        urlIndex[j + 1] = index;
    }

    // free
    free(curMatrix);
    for (int i = 0; i < numberofurl; i++) {
        free(matrix[i]);
        free(reversematrix[i]);
    }
    free(matrix);
    free(reversematrix);
}

void writetext(char **urlarray, int urlIndex[], int urlsDegree[], double vector[], int numberofurl) {
    //char line[MAX_STRING];
    FILE *fp;
    // open file
    if ((fp = fopen("pagerankList.txt", "w")) != NULL) {
        //  foreach line to write
        for (int i = 0; i < numberofurl; i++) {
            // format line to write
            fprintf(fp, "%s, %d, %.7f\n", urlarray[urlIndex[i]], urlsDegree[urlIndex[i]],
                    vector[urlIndex[i]]);
        }
    }
    /* close the file*/
    fclose(fp);
}

int main(int argc, char *argv[]) {
    assert(argc == 4);

    //get all the parameters
    double division = strtod(argv[1], NULL);
    double diffPR = strtod(argv[2], NULL);
    int maxIterations = atoi(argv[3]);

    // read from collection.txt
    DLListStr collectionUrl = newDLListStr();
    readCollection(collectionUrl);
    //showDLListStr(collectionUrl);

    // get the length of list
    int numberofurl = lengthDLListStr(collectionUrl);

    //convert list to array
    char **urlarray = malloc(numberofurl * sizeof(char *));
    convertDLLtoArray(collectionUrl, urlarray);


    Graph g = newGraph(numberofurl);
    readSection1(urlarray, g, numberofurl);

    // create the probability vector
    double *vector;
    vector = malloc(numberofurl * sizeof(double));
    //printf("hello\n");
    for (int i = 0; i < numberofurl; i++) {
        vector[i] = 1.0 / numberofurl;
        //printf("%f\n", vector[i]);
    }

    // get the degree of the graph
    int *urlsDegree = malloc(numberofurl * sizeof(int));

    // get the index of each url
    int *urlIndex = malloc(numberofurl * sizeof(int));
    for (int i = 0; i < numberofurl; i++) {
        urlIndex[i] = i;
    }

    // pagerank the urls
    pagerank(division, diffPR, maxIterations, vector, urlsDegree, g, numberofurl, urlIndex);

    // write the result to the txt
    writetext(urlarray, urlIndex, urlsDegree, vector, numberofurl);

    // free
    freeDLListStr(collectionUrl);
    freeGraph(g);
    free(urlsDegree);
    free(vector);

    for (int i = 0; i < numberofurl; i++) {
        free(urlarray[i]);
    }
    free(urlarray);
    return 0;
}