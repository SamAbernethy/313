#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

// ******************************
// ANSI FUNCTION DECLARATIONS
// FUNCTIONS ARE IN MAIN.H
// ******************************

void initialize(double *maxtime, double *timestep, double *initialtime, double *introducedtime, int *outputs);

int checksize(int sites[][EDEN_MAX], int whattype);
void output(int sites[][EDEN_MAX], FILE *eden_out, FILE *perimeters_out, int middle, int whattype);
void initializegrid(int sites[][EDEN_MAX], int xdisp, int ydisp, int whattype);
void resetperimetervalues(int perimeterxvalues[], int perimeteryvalues[]);
void updatecluster(int sites[][EDEN_MAX], int numb, int perimeterxvalues[], int perimeteryvalues[]);
int pseudorandom(int modulus);
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]);
int numberofperimeters(int sites[][EDEN_MAX]);
void massandradius(FILE *dimension_out, int sites[][EDEN_MAX], int whattype, int middle);

void virussplit(int virus[][EDEN_MAX], int i, int j, int numberofsplit);
void NoMoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit);
void MoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit, int chanceofmovement);
void SuperVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit);

// ********************************************
// In all code, 1 is cluster, 3 is virus, 10 is dead cluster
// ********************************************

int main(void)
{
    // ****************************************
    // START OF EDEN
    // ****************************************

    /* background
     * In version A, a to-be-infected cell is chosen with same probability from all uninfected cells adjacent to the cluster. In version B, an infection path from all possible paths from infected to adjacent uninfected cells is chosen with the same probability (the original Eden model). In version C, firstly a boundary cell of the cluster is randomly chosen, then an uninfected adjacent cell is randomly chosen to be infected. */

    srand((unsigned)time(NULL)); // seeds the pseudorandom generator
    printf("Starting Eden Cluster Model: \n");

    // *****************************************
    // OUTPUT FILES
    // ****************************************

    // Perimeters of Eden
    FILE *perimeters_out;
    perimeters_out = fopen("perimeters.dat", "w");

    // Filled lattice points for Eden
    FILE *eden_out;
    eden_out = fopen("eden.dat", "w");

    // third and two thirds of the way
    FILE *thirdeden_out;
    FILE *twothirdseden_out;
    thirdeden_out = fopen("thirdeden.dat", "w");
    twothirdseden_out = fopen("twothirdseden.dat", "w");

    // filled virus points
    FILE *virus_out;
    virus_out = fopen("virus.dat", "w");

    FILE *thirdvirus_out;
    FILE *twothirdsvirus_out;
    thirdvirus_out = fopen("thirdvirus.dat", "w");
    twothirdsvirus_out = fopen("twothirdsvirus.dat", "w");

    // virus perimeters
    FILE *virusperimeters_out;
    virusperimeters_out = fopen("virusperimeters.dat", "w");

    // dead cells
    FILE *deadcells_out;
    deadcells_out = fopen("deadcells.dat", "w");

    FILE *thirddeadcells_out;
    FILE *twothirdsdeadcells_out;
    thirddeadcells_out = fopen("thirddeadcells.dat", "w");
    twothirdsdeadcells_out = fopen("twothirdsdeadcells.dat", "w");

    // tracking size of cluster
    FILE *size_out;
    size_out = fopen("size.dat", "w");

    // *****************************************
    // VARIABLE DECLARATIONS
    // *****************************************

    int i; // index, usually the x axis of array
    int j; // index, usually the y axis of array
    int middle = (EDEN_MAX - 1) / 2; // middle of the grid, gets subtracted to center at origin
    double maxtime, timestep, initialtime, introducedtime; // commented in initialize function
    int outputs; // number of data points we want to track for cluster size over time

    initialize(&maxtime, &timestep, &initialtime, &introducedtime, &outputs);

    double newt; // new time, to compare to old time for various outputs
    int numb; // number of perimeter sites
    double dt = timestep; // added to t every time a cluster site is filled
    double t = initialtime; // starts here, gets incremented by dt

    // initialize sites with a 1 in center. This will contain eden cluster points (1) and perimeter points (2)
    int sites[EDEN_MAX][EDEN_MAX];
    initializegrid(sites, middle, middle, 1);

    // initialize virus with a 3 in center
    int virus[EDEN_MAX][EDEN_MAX];
    initializegrid(virus, middle, middle, 3);

    int perimeterxvalues[EDEN_MAX]; // holds the values of x of all perimeters of cluster
    int perimeteryvalues[EDEN_MAX]; // holds the values of y of all perimeters of cluster

    /* Give it a certain amount of time to run for, passed in from main.in
     * A new perimeter site is filled every dt, and it gets faster based on how many perimeters there are
     * Bacteriophage (virus) introduced to kill bacteria from within
     * */

    /* pseudocode for bacteriophage
     * start with a virus at the origin, just like eden model
     * have it start out with slower growth, but have it exponentially rise faster
     * give it a 10% factor that slows it down initially, but then it splits into two or more
     * 3s will be virus sites
     * */

    // eden is 1, eden perimeter is 2, virus is 3, dead is 10

    // ******************************************
    // MAIN LOOP
    // ******************************************

    while (t < maxtime) {
        if (t >= introducedtime) {
            // virus attack with no movement
            //NoMoveVirus(virus, sites, 10, 2); // chanceofsplit, number of split

            // virus attack with movement
            //MoveVirus(virus, sites, 3, 3, 100); // chanceofsplit, number of split, chance of movement

            // killer virus attack
            SuperVirus(virus, sites, 10);
        }

        resetperimetervalues(perimeterxvalues, perimeteryvalues);
        numb = updateperimeters(sites, perimeterxvalues, perimeteryvalues);

        // if the cluster has been killed, exit the while loop
        if (numb == 0) {
            printf("Time until death of bacteria was %lf\n", t);
            printf("Virus was introduced at time %lf\n", introducedtime);
            printf("Lifetime was %lf\n", t - introducedtime);
            break;
        }

        updatecluster(sites, numb, perimeterxvalues, perimeteryvalues);
        printf("Current number of perimeters: %d\n", numb);

        dt = (double) timestep / numb;
        newt = t + dt;
        if ((newt > (maxtime/3.) && (t < (maxtime/3.)))) {
            output(sites, thirdeden_out, perimeters_out, middle, 1);
            output(sites, thirddeadcells_out, perimeters_out, middle, 10);
            output(virus, thirdvirus_out, virusperimeters_out, middle, 3);
        }
        if ((newt > (2.*maxtime/3.) && (t < (2.*maxtime/3.)))) {
            output(sites, twothirdseden_out, perimeters_out, middle, 1);
            output(sites, twothirdsdeadcells_out, perimeters_out, middle, 10);
            output(virus, twothirdsvirus_out, virusperimeters_out, middle, 3);
        }

        int size;
        for (i = 1; i <= outputs; i++) {
            if ((newt > ((double) i * maxtime/outputs)) && (t < ((double) i * maxtime/outputs ))) {
                size = checksize(sites, 1);
                fprintf(size_out, "%lf\t%d\n", t, size);
            }
        }

        t = newt;
        printf("Time left is %lf\n", (maxtime - t));
    }

    printf("\n RESULTS: \n\n");
    printf("Number of perimeter sites is %d\n", numb);
    output(sites, eden_out, perimeters_out, middle, 1);
    output(sites, deadcells_out, perimeters_out, middle, 10);
    output(virus, virus_out, virusperimeters_out, middle, 3);

    // check the dimensionality of the eden cluster
    FILE *dimension_out;
    dimension_out = fopen("dimension.dat", "w");
    massandradius(dimension_out, sites, 1, middle);

    FILE *virusdim_out;
    virusdim_out = fopen("virusdim.dat", "w");
    massandradius(virusdim_out, virus, 3, middle);

    return 0;
}
