#ifndef MAIN_H
#define MAIN_H

#define EDEN_MAX 1001 // dimensions of grid for Eden
#define STRINGLENGTH 25 // maximum number of characters in a string
#define TRIALS 1000 // number of points for Eden
#define MAX_TIME 30
#define TIME_STEP 1

// *****************************************
// VARIABLE DECLARATIONS
// *****************************************

int i; // index, usually the x axis of array
int j; // index, usually the y axis of array
int middle = (EDEN_MAX - 1) / 2; // middle of the grid, gets subtracted to center at origin

// ********************************************
// FUNCTIONS USED BY EDEN
// ********************************************

// initializes all points to be 0's, except a 1 at a given point
void initializegrid(int sites[][EDEN_MAX], int xdisp, int ydisp, int whattype) {
    int i, j;
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            sites[i][j] = 0;
        }
    }
    // 1 for eden
    // 3 for virus
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

// outputs from a grid (labelled sites for now)
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

// generates a random number from 0 to (modulus - 1)
int pseudorandom(int modulus) {
    int r = rand() % modulus;
    return r;
}

// updates perimeters to be 2s if they have a filled spot nearby
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]) {
    int n = 0;
    int i, j;

    for (i = 1; i < (EDEN_MAX-1); i++) {
        for (j = 1; j < (EDEN_MAX-1); j++) {
            if ((sites[i][j] == 2) && !(( (sites[i][j-1] == 1) || (sites[i][j+1] == 1) || (sites[i-1][j] == 1) || (sites[i+1][j] == 1) ))) {
                sites[i][j] = 0;
            }
        }
    }

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

void massandradius(FILE *dimension_out, int sites[][EDEN_MAX], int whattype) {
    int radius, mass, i, j;
    int rmax = 0;

    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if ((sites[i][j] == whattype) && (sqrt((i-middle)*(i-middle) + (j-middle)*(j-middle)) > rmax)) {
                    rmax = sqrt((i-middle)*(i-middle) + (j-middle)*(j-middle));
            }
        }
    }
    printf("Maximum radius is %d\n", rmax);

    for (radius = 2; radius <= (rmax/2); radius++) {
        mass = 0;
        for (i = 0; i < EDEN_MAX; i++) {
            for (j = 0; j < EDEN_MAX; j++) {
                if ((((i-middle)*(i-middle) + (j-middle)*(j-middle)) <= radius*radius) && (sites[i][j] == whattype)) {
                    mass++;
                }
            }
        }
        fprintf(dimension_out, "%lf\t%lf\n", log((double) radius), log((double) mass));
    }
}

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

void NoMoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit) {
    /* slow down by a factor of 10, then take a given virus location and split it into two more viruses
     * this means make virus[][] be a 3
     * compare those two virus locations to the sites, and if they match, make sites be 0
     */
    int dowesplit; // yes if dowesplit is 1, so chance is 1/chanceofsplit
    int i, j;

    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
            if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                dowesplit = pseudorandom(chanceofsplit);
                if (dowesplit == 1) {
                    virussplit(virus, i, j, numberofsplit);
                    sites[i][j] = 10; // 10 means very dead now
                    virus[i][j] = 0;
                }
            }
        }
    }
    return;
}

void MoveVirus(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit, int numberofsplit, int chanceofmovement) {
    int dowesplit;
    int shift;
    int i, j;
    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
            if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                dowesplit = pseudorandom(chanceofsplit);
                if (dowesplit == 1) {
                    virussplit(virus, i, j, numberofsplit);
                    sites[i][j] = 10; // 10 means dead, no bacteria can go there
                    virus[i][j] = 0;
                }
            }
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

void Cvirusattack(int virus[][EDEN_MAX], int sites[][EDEN_MAX]) {
    int i, j;
    int dowesplit;
    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
             if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                 dowesplit = pseudorandom(3);
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
