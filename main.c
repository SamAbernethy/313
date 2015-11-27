#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

// ******************************
// ANSI FUNCTION DECLARATIONS
// FUNCTIONS ARE IN MAIN.H
// ******************************

int checkifperimeter(int DLA_sites[][EDEN_MAX], int randomwalker, int randomwalkery, int dlaperimeterxvalues[], int dlaperimeteryvalues[]);
void randomwalk(int DLA_sites[][EDEN_MAX], int randomwalkerx[], int randomwalkery[], int dlaperimeterxvalues[], int dlaperimeteryvalues[]);

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
    srand((unsigned)time(NULL)); // seeds the pseudorandom generator

    // ************************************
    // DLA CLUSTER
    // ************************************

    // Pseudocode
    /* make a grid that is EDEN_MAX by EDEN_MAX
     *  initialize it to be all 0s, but with a 1 in the very middle
     * run through TRIALS times, doing:
     * resetperimetervalues, which sets all perimeter values to be 0s again
     * update the perimeters which counts and stores all perimeter sites
     *
     *
     *
     *
     *
     * */


    printf("This is the start of our random walk\n");

    FILE *dla_out;
    dla_out = fopen("dla.dat", "w");

    int DLA_sites[EDEN_MAX][EDEN_MAX];
    initializegrid(DLA_sites, middle, middle);

    int dlaperimeterxvalues[EDEN_MAX];
    int dlaperimeteryvalues[EDEN_MAX];

    int randomwalkerx[MAX_ITERATIONS];
    int randomwalkery[MAX_ITERATIONS];

    for (i = 0; i < DLA_TRIALS; i++) {
        resetperimetervalues(dlaperimeterxvalues, dlaperimeteryvalues);

        int dlanumber = updateperimeters(DLA_sites, dlaperimeterxvalues, dlaperimeteryvalues);
        //printf("Number of perimeter sites: %d\n", dlanumber);

        randomwalk(DLA_sites, randomwalkerx, randomwalkery, dlaperimeterxvalues, dlaperimeteryvalues);
    }

    output(DLA_sites, dla_out, middle);


    // ****************************************
    // START OF EDEN
    // ****************************************

    /* psuedo code for eden
        take a 101 by 101 array and put a 1 at (50,50), 0s elsewhere
        take all points that are 1s, and label all 0 points around it as 2s "periemter sites"
        index all of the 2s, keep that number, r
        generate random number from 0 to r-1 and make that 2 point a 1
        repeat!
      */
/*
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
    */
    return 0;
}
