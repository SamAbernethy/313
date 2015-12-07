#ifndef MAIN_H
#define MAIN_H

#define EDEN_MAX 1001 // dimensions of grid for Eden
// used as a define since it is used as the dimension of every array for all functions

// ********************************************
// FUNCTIONS USED BY EDEN
// ********************************************

// this function initializes the time variables, from main.in
void initialize(double *maxtime, double *timestep, double *initialtime, double *introducedtime, int *outputs) {
    printf("\n Max time -> ");
    scanf("%lf", maxtime);
    printf("%lf\n", *maxtime); // maximum amount of time to run code for

    printf("\n Time step -> ");
    scanf("%lf", timestep);
    printf("%lf\n", *timestep); // time step that gets divided by number of perimeters to give dt

    printf("\n Initial time -> ");
    scanf("%lf", initialtime);
    printf("%lf\n", *initialtime); // starts at this time (0)

    printf("\n Introduced time -> ");
    scanf("%lf", introducedtime);
    printf("%lf\n", *introducedtime); // time at which a virus is introduced to existing cluster

    printf("\n Number of outputs -> ");
    scanf("%d", outputs);
    printf("%d\n", *outputs);
}

// initializes all points to be 0s, except for at the (xdisp, ydisp)
// note that "whattype" can be a 1 (cluster) or a 3 (virus)
void initializegrid(int sites[][EDEN_MAX], int xdisp, int ydisp, int whattype) {
    int i, j;
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            sites[i][j] = 0;
        }
    }
    sites[xdisp][ydisp] = whattype;
}

// resets all perimeter values to 0
void resetperimetervalues(int perimeterxvalues[], int perimeteryvalues[]) {
    int j;
    for (j = 0; j < EDEN_MAX; j++) {
        perimeterxvalues[j] = 0;
        perimeteryvalues[j] = 0;
    }
}

// updates the cluster given a randomly chosen perimeter point
void updatecluster(int sites[][EDEN_MAX], int numb, int perimeterxvalues[], int perimeteryvalues[]) {
    int whichperimeter = pseudorandom(numb);
    //printf("Random number from 0 to (%d - 1) was %d \n", numb, whichperimeter);
    //printf("That means we select the point %d %d \n", perimeterxvalues[whichperimeter], perimeteryvalues[whichperimeter]);
    int changingxvalue = perimeterxvalues[whichperimeter];
    int changingyvalue = perimeteryvalues[whichperimeter];
    sites[changingxvalue][changingyvalue] = 1;
    //printf("I just made %d %d a filled data point \n", changingxvalue, changingyvalue);
}

// outputs from a grid
void output(int sites[][EDEN_MAX], FILE *eden_out, FILE *perimeters_out, int middle, int whattype) {
    int i, j;
    int n = 0;
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if (sites[i][j] == whattype) {
                fprintf(eden_out, "%d\t%d\n", (i-middle), (j-middle));
                n++;
                // printf("outputting: %d %d \n", i, j);
            }
            if (sites[i][j] == 2) {
                fprintf(perimeters_out, "%d\t%d\n", (i-middle), (j-middle));
            }
        }
    }
    printf("Number of lattice points of type %d is %d\n", whattype, n);
    return;
}

int checksize(int sites[][EDEN_MAX], int whattype) {
    int i, j;
    int n = 0;
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if (sites[i][j] == whattype) {
                n++;
            }
        }
    }
    return n;
}

// generates a random number from 0 to (modulus - 1), based on a seed from the current time
int pseudorandom(int modulus) {
    int r = rand() % modulus;
    return r;
}

// updates perimeters to be 2s if they have a filled spot nearby
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]) {
    int n = 0;
    int i, j;

    // checks to make sure that a "used-to-be" perimeter site is still a perimeter site, given virus attacks
    for (i = 1; i < (EDEN_MAX-1); i++) {
        for (j = 1; j < (EDEN_MAX-1); j++) {
            if ((sites[i][j] == 2) && !(( (sites[i][j-1] == 1) || (sites[i][j+1] == 1) || (sites[i-1][j] == 1) || (sites[i+1][j] == 1) ))) {
                sites[i][j] = 0;
            }
        }
    }

    // runs through all points, and if it has a filled neighbour, then it is a perimeter site
    for (i = 1; i < (EDEN_MAX-1); i++) {
        for (j = 1; j < (EDEN_MAX-1); j++) {
            if (((sites[i][j] == 0) || (sites[i][j] == 2) ) && ( (sites[i][j-1] == 1) || (sites[i][j+1] == 1) || (sites[i-1][j] == 1) || (sites[i+1][j] == 1) ) ) {
                sites[i][j] = 2;
                perimeterxvalues[n] = i;
                perimeteryvalues[n] = j;
                //printf("Perimeter is (%d, %d) \n", i, j);
                n++;
            }
        }
    }
    return n;
}

// counts up the number of perimeter sites, so that we can choose one randomly
int numberofperimeters(int sites[][EDEN_MAX]) {
    int numberofperimeters = 0;
    int i, j;
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if (sites[i][j] == 2) {
                numberofperimeters++;
            }
        }
    }
    return numberofperimeters;
}

// graphs the mass and the radius (on a log-log plot) for a cluster
void massandradius(FILE *dimension_out, int sites[][EDEN_MAX], int whattype, int middle) {
    int radius, mass, i, j;
    int rmax = 0;

    // finding the maximum radius
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if ((sites[i][j] == whattype) && (sqrt((i-middle)*(i-middle) + (j-middle)*(j-middle)) > rmax)) {
                    rmax = sqrt((i-middle)*(i-middle) + (j-middle)*(j-middle));
            }
        }
    }
    printf("Maximum radius is %d\n", rmax);

    // finding mass for a given radius
    for (radius = 2; radius <= (rmax/2); radius++) {
        mass = 0;
        for (i = 0; i < EDEN_MAX; i++) {
            for (j = 0; j < EDEN_MAX; j++) {
                if ((((i-middle)*(i-middle) + (j-middle)*(j-middle)) <= radius*radius) && (sites[i][j] == whattype)) {
                    mass++;
                }
            }
        }
        // outputs in a log-log graph
        fprintf(dimension_out, "%lf\t%lf\n", log((double) radius), log((double) mass));
    }
}

// splits a virus into a number of directions given by numberofsplit
void virussplit(int virus[][EDEN_MAX], int i, int j, int numberofsplit) {
    int k;
    int movement;
    for (k = 1; k <= numberofsplit; k++) {
        movement = pseudorandom(4); // can move or stay still
        if (movement == 0) {
            virus[i-1][j] = 3;
        }
        else if (movement == 1) {
            virus[i+1][j] = 3;
        }
        else if (movement == 2) {
            virus[i][j-1] = 3;
        }
        else if (movement == 3) {
            virus[i][j+1] = 3;
        }
    }
    return;
}

// virus attack without random movement of the virus itself - it can only expand when it splits on a cluster site
// note that 1 / chanceofsplit is the odds that a given virus on a filled site will split
// note that numberofsplit is the number of new viruses that get split into
void NoMoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit) {
    int dowesplit; // yes if dowesplit is 1, so chance is 1/chanceofsplit
    int i, j;

    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
            if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                dowesplit = pseudorandom(chanceofsplit);
                if (dowesplit == 1) {
                    virussplit(virus, i, j, numberofsplit);
                    sites[i][j] = 10; // 10 means dead cell now
                    virus[i][j] = 0;
                }
            }
        }
    }
    return;
}

// virus attack where the virus moves randomly, as well as splitting on filled cluster sites
// 1 / chanceofmovement is the odds that a given virus moves in a random direction at each time step
void MoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit, int chanceofmovement) {
    int dowesplit;
    int shift;
    int i, j;
    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
            // splitting
            if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                dowesplit = pseudorandom(chanceofsplit);
                if (dowesplit == 1) {
                    virussplit(virus, i, j, numberofsplit);
                    sites[i][j] = 10; // 10 means dead, no bacteria can go there
                    virus[i][j] = 0;
                }
            }
            // moving
            else if ((virus[i][j] == 3) && (sites[i][j] != 1)) {
                shift = pseudorandom(chanceofmovement);
                if (shift == 0) {
                    virus[i-1][j] = 3;
                    virus[i][j] = 0;
                }
                if (shift == 1) {
                    virus[i+1][j] = 3;
                    virus[i][j] = 0;
                }
                if (shift == 2) {
                    virus[i][j-1] = 3;
                    virus[i][j] = 0;
                }
                if (shift == 3) {
                    virus[i][j+1] = 3;
                    virus[i][j] = 0;
                }
            }
        }
    }
    return;
}

void SuperVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit) {
    int i, j;
    int dowesplit;
    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
             if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                 dowesplit = pseudorandom(chanceofsplit);
                 if (dowesplit == 0) {
                     virus[i+1][j] = 3;
                     virus[i-1][j] = 3;
                     virus[i][j-1] = 3;
                     virus[i][j+1] = 3;
                     sites[i][j] = 10;
                     virus[i][j] = 0;
                 }
             }
         }
    }
    return;
}

#endif // MAIN_H
