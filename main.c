#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

// ******************************
// ANSI FUNCTION DECLARATIONS
// FUNCTIONS ARE IN MAIN.H
// ******************************

void output(int sites[][EDEN_MAX], FILE *fp_out, int middle);
void initializegrid(int sites[][EDEN_MAX], int xdisp, int ydisp);
void resetperimetervalues(int perimeterxvalues[], int perimeteryvalues[]);
void updatecluster(int sites[][EDEN_MAX], int numb, int perimeterxvalues[], int perimeteryvalues[]);
int pseudorandom(int modulus);
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]);
int numberofperimeters(int sites[][EDEN_MAX]);
void massandradius(FILE *dimension_out, int sites[][EDEN_MAX]);

int main(void)
{
    /*printf("Hello World!\n");
    printf("This is the start of our project \n");
    FILE *fp_out;
    fp_out = fopen("cluster.dat", "w");
    srand((unsigned)time(NULL));
    int x[MAX_ITERATIONS];
    int y[MAX_ITERATIONS];
    int l = 0;
    for (l = 0; l < MAX_ITERATIONS; l++) {
        x[l] = 0;
        y[l] = 0;
    }
    int tempx = 0;
    int tempy = 0;
    int walkiterations = 0;
    for (walkiterations = 1; walkiterations < MAX_ITERATIONS; walkiterations++) {
        output(fp_out, tempx, tempy);
        int j = pseudorandom(4);
        printf("Random integer is %d\n", j);
        if (j == 0) {
            x[walkiterations] = x[walkiterations-1] + 1;
            printf("New x is %d \n", x[walkiterations]);
            y[walkiterations] = y[walkiterations-1];
        }
        else if (j == 1) {
            x[walkiterations] = x[walkiterations-1] - 1;
            y[walkiterations] = y[walkiterations-1];
        }
        else if (j == 2) {
            y[walkiterations] = y[walkiterations-1] + 1;
            x[walkiterations] = x[walkiterations-1];
        }
        else {
            y[walkiterations] = y[walkiterations-1] - 1;
            x[walkiterations] = x[walkiterations-1];
        }
        tempx = x[walkiterations];
        tempy = y[walkiterations];
    }
    */

    /* psuedo code for eden
        take a 101 by 101 array and put a 1 at (50,50), 0s elsewhere
        take all points that are 1s, and label all 0 points around it as 2s "periemter sites"
        index all of the 2s, keep that number, r
        generate random number from 0 to r-1 and make that 2 point a 1
        repeat!
      */

    srand((unsigned)time(NULL));
    printf("Starting Eden Cluster Model: \n");
    FILE *eden_out;
    FILE *killer_out;
    killer_out = fopen("killer.dat", "w");
    eden_out = fopen("eden.dat", "w");

    int sites[EDEN_MAX][EDEN_MAX];
    initializegrid(sites, middle, middle);

    int killer[EDEN_MAX][EDEN_MAX];
    initializegrid(killer, (middle+20), (middle+20));

    int perimeterxvalues[EDEN_MAX];
    int perimeteryvalues[EDEN_MAX];
    int killerperimeterxvalues[EDEN_MAX];
    int killerperimeteryvalues[EDEN_MAX];

    for (i = 0; i < TRIALS; i++) { // Do this TRIALS times:
        resetperimetervalues(perimeterxvalues, perimeteryvalues);
        resetperimetervalues(killerperimeterxvalues, killerperimeteryvalues);

        int numb = updateperimeters(sites, perimeterxvalues, perimeteryvalues);
        //printf("Current number of perimeters: %d\n", numb);
        int killernumb = updateperimeters(killer, killerperimeterxvalues, killerperimeteryvalues);
        //printf("Number of perimeters for killer: %d\n", killernumb);

        updatecluster(sites, numb, perimeterxvalues, perimeteryvalues);
        updatecluster(killer, killernumb, killerperimeterxvalues, killerperimeteryvalues);
        if (i % 100 == 0) { // to see where we are
            printf("At step %d of %d\n", i, TRIALS);
        }
    }
    output(sites, eden_out, middle);
    output(killer, killer_out, middle);

    FILE *dimension_out;
    dimension_out = fopen("dimension.dat", "w");

    // check the dimensionality of the eden cluster
    massandradius(dimension_out, sites);

    return 0;
}
