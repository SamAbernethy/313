#ifndef MAIN_H
#define MAIN_H

#define EDEN_MAX 501 // dimensions of grid for Eden
#define STRINGLENGTH 25 // maximum number of characters in a string
#define TRIALS 1000 // number of points for Eden
#define MAX_TIME 20
#define TIME_STEP 1

// *****************************************
// VARIABLE DECLARATIONS
// *****************************************
int i; // index, usually the x axis of array
int j; // index, usually the y axis of array
int middle = (EDEN_MAX - 1) / 2; // middle of the grid, gets subtracted to center at origin

// ******************************************
// FUNCTIONS
// ******************************************

/*int checkifperimeter(int DLA_sites[][EDEN_MAX], int randomwalkerx, int randomwalkery, int dlaperimeterxvalues[], int dlaperimeteryvalues[]) {
    int yesorno = 0;
    int j;
    while (yesorno == 0) {
        for (j = 0; j < EDEN_MAX; j++) {
            if (((randomwalkerx != 0) || (randomwalkery != 0)) && (randomwalkerx == dlaperimeterxvalues[j]) && (randomwalkery == dlaperimeteryvalues[j])) {
                DLA_sites[randomwalkerx][randomwalkery] = 1;
                printf("Making %d %d a filled site \n", randomwalkerx, randomwalkery);
                yesorno = 1;
            }
        }
    }
    return yesorno;
}

void randomwalk(int DLA_sites[][EDEN_MAX], int randomwalkerx[], int randomwalkery[], int dlaperimeterxvalues[], int dlaperimeteryvalues[]) {
    int i;
    int x[MAX_ITERATIONS]; // x location for random walk
    int y[MAX_ITERATIONS]; // y location for random walk

    for (i = 0; i < MAX_ITERATIONS; i++) {
        x[i] = 0;
        y[i] = 0;
        randomwalkerx[i] = 0;
        randomwalkery[i] = 0;
    }

    // starting it at (20,20) for now
    x[0] = 20;
    y[0] = 20;

    int foundit = 0;
    while (foundit == 0) {
        for (i = 0; i < MAX_ITERATIONS; i++) {
            int j = pseudorandom(4);
            if (j == 0) {
                x[i] = x[i-1] + 1;
                y[i] = y[i-1];
            }
            if (j == 1) {
                x[i] = x[i-1] - 1;
                y[i] = y[i-1];
            }
            if (j == 2) {
                x[i] = x[i-1];
                y[i] = y[i-1] - 1;
            }
            if (j == 3) {
                x[i] = x[i-1];
                y[i] = y[i-1] + 1;
            }
            int tempx = x[i];
            int tempy = y[i];
            int a = checkifperimeter(DLA_sites, tempx, tempy, dlaperimeterxvalues, dlaperimeteryvalues);
            if (a == 1) {
                foundit = 1;
                printf("Found one!");
            }
        }
    }
    return;
}
*/

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
            if (  ( (sites[i][j] == 0) || (sites[i][j] == 2) ) && ( (sites[i][j-1] == 1) || (sites[i][j+1] == 1) || (sites[i-1][j] == 1) || (sites[i+1][j] == 1) ) ) {
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

    for (radius = 1; radius <= (rmax/2); radius++) {
        mass = 0;
        for (i = 0; i < EDEN_MAX; i++) {
            for (j = 0; j < EDEN_MAX; j++) {
                if (((i*i + j*j) < radius*radius) && (sites[i+middle][j+middle] == whattype)) {
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
        movement = pseudorandom(5); // can move or stay still
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
        // if movement is 4, it stays still
    }
    return;
}

void Avirusattack(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit) {
    /* slow down by a factor of 10, then take a given virus location and split it into two more viruses
     * this means make virus[][] be a 3
     * compare those two virus locations to the sites, and if they match, make sites be 0
     */
    int dowesplit; // yes if dowesplit is 0, so chance is 1/chanceofsplit
    int i, j;

    for (i = 0; i < EDEN_MAX; i++) {
        for (j = 0; j < EDEN_MAX; j++) {
            if ((virus[i][j] == 3) && (sites[i][j] == 1)) {
                // do we split this one?
                dowesplit = pseudorandom(chanceofsplit);
                //dowesplit = 0;
                if (dowesplit == 0) {
                    virussplit(virus, i, j, 2);
                    sites[i][j] = 0;
                    //virus[i][j] = 0;
                }
            }

        }
    }
    return;
}

void Bvirusattack(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit) {
    int dowesplit;
    int shift;
    /*else if ((virus[i][j] == 3) && (sites[i][j] != 1)) {
        shift = pseudorandom(100);
        // if 4, don't move
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
    }*/



    return;
}

void Cvirusattack(int virus[][EDEN_MAX], int sites[][EDEN_MAX], int chanceofsplit) {
    int dowesplit;





    return;
}





#endif // MAIN_H
