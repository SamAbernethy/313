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

void output(int sites[][EDEN_MAX], FILE *eden_out, FILE *perimeters_out, int middle, int whattype);
void initializegrid(int sites[][EDEN_MAX], int xdisp, int ydisp, int whattype);
void resetperimetervalues(int perimeterxvalues[], int perimeteryvalues[]);
void updatecluster(int sites[][EDEN_MAX], int numb, int perimeterxvalues[], int perimeteryvalues[]);
int pseudorandom(int modulus);
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]);
int numberofperimeters(int sites[][EDEN_MAX]);
void massandradius(FILE *dimension_out, int sites[][EDEN_MAX], int whattype);

void virusattack(int virus[][EDEN_MAX], int sites[][EDEN_MAX]   );

int main(void)
{
    srand((unsigned)time(NULL)); // seeds the pseudorandom generator

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

    /* background
     *
     *
     *
     * In version A, a to-be-infected cell is chosen with same probability from all uninfected cells adjacent to the cluster. In version B, an infection path from all possible paths from infected to adjacent uninfected cells is chosen with the same probability (the original Eden model). In version C, firstly a boundary cell of the cluster is randomly chosen, then an uninfected adjacent cell is randomly chosen to be infected. */

    printf("Starting Eden Cluster Model: \n");

    // Perimeters of Eden
    FILE *perimeters_out;
    perimeters_out = fopen("perimeters.dat", "w");

    // Filled lattice points for Eden
    FILE *eden_out;
    eden_out = fopen("eden.dat", "w");

    // filled virus points
    FILE *virus_out;
    virus_out = fopen("virus.dat", "w");

    // virus perimeters
    FILE *virusperimeters_out;
    virusperimeters_out = fopen("virusperimeters.dat", "w");

    // initialize sites with a 1 in center
    int sites[EDEN_MAX][EDEN_MAX];
    initializegrid(sites, middle, middle, 1);

    // initialize virus with a 3 in center
    int virus[EDEN_MAX][EDEN_MAX];
    initializegrid(virus, middle, middle, 3);

    int perimeterxvalues[EDEN_MAX];
    int perimeteryvalues[EDEN_MAX];
    int virusperimeterxvalues[EDEN_MAX];
    int virusperimeteryvalues[EDEN_MAX];

    // pseduo code for implementing time
    /* Give it a certain amount of time to run for
     * a new sites is filled every dt, and it gets faster based on how many particles there are
     * or based on how many perimeters there are! That could be more interesting
     * bacteriophage to kill bacteria from within
     * */

    /* pseudocode for bacteriophage
     * start with a virus at the origin, just like eden model
     * have it start out with slower growth, but have it exponentially rise faster
     * it can only exist where a bacteria WAS (before being killed)
     * let's say virus can go diagonal too!
     * give it a 10% factor that slows it down initially, but then it splits into two more
     * 3s will be virus sites
     * we'll need a function that compares virus to sites
     * */

    // eden is 1, eden per is 2, virus is 3, virus per is 4

    int numb;
    double dt = TIME_STEP;
    double t = 0;
    while (t < MAX_TIME) {
        resetperimetervalues(perimeterxvalues, perimeteryvalues);
        numb = updateperimeters(sites, perimeterxvalues, perimeteryvalues);
        updatecluster(sites, numb, perimeterxvalues, perimeteryvalues);
        //printf("Current number of perimeters: %d\n", numb);

        /* secondary one if needed
        resetperimetervalues(virusperimeterxvalues, virusperimeteryvalues);
        int virusnumb = updateperimeters(virus, virusperimeterxvalues, virusperimeteryvalues);
        //printf("Number of perimeters for virus: %d\n", virusnumb);
        updatecluster(virus, virusnumb, virusperimeterxvalues, virusperimeteryvalues);
        */

        virusattack(virus, sites);

        dt = (double) TIME_STEP / numb;
        t = t + dt;
        printf("Time left is %lf\n", (MAX_TIME - t));
    }

    printf("\n RESULTS: \n\n");
    printf("Number of perimeter sites is %d\n", numb);
    output(sites, eden_out, perimeters_out, middle, 1);
    output(virus, virus_out, virusperimeters_out, middle, 3);





    // check the dimensionality of the eden cluster
    FILE *dimension_out;
    dimension_out = fopen("dimension.dat", "w");
    massandradius(dimension_out, sites, 1);

    FILE *virusdim_out;
    virusdim_out = fopen("virusdim.dat", "w");
    massandradius(virusdim_out, virus, 3);

    return 0;
}
