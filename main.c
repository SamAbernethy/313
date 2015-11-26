#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/* PSUEDO CODE
 * Take any occupied site that has an empty neighbour randomly
 * Pick an unoccopied neighbour randomly
 * reiterate up to a maximum number of times
 * */

#define MAX_ITERATIONS 10
#define EDEN_MAX 1001
#define TRIALS 1000

void output(FILE *fp_out, int x, int y);
int pseudorandom(int modulus);
int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]);
int numberofperimeters(int sites[][EDEN_MAX]);

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
    printf("Eden code attempt\n");
    FILE *eden_out;
    FILE *killer_out;
    killer_out = fopen("killer.dat", "w");
    eden_out = fopen("eden.dat", "w");
    int middle = (EDEN_MAX - 1) / 2; // EDEN_MAX must be odd
    printf("%d\n", middle);
    int i, j;

    int sites[EDEN_MAX][EDEN_MAX];
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            sites[i][j] = 0;
        }
    }

    int killer[EDEN_MAX][EDEN_MAX];
    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            killer[i][j] = 0;
        }
    }

    sites[middle][middle] = 1;
    killer[(middle+20)][(middle+20)] = 1;

    int maxdimension = EDEN_MAX;
    int perimeterxvalues[maxdimension];
    int perimeteryvalues[maxdimension];
    int killerperimeterxvalues[maxdimension];
    int killerperimeteryvalues[maxdimension];

    for (i = 0; i < TRIALS; i++) {
        for (j = 0; j < maxdimension; j++) {
            perimeterxvalues[j] = 0;
            perimeteryvalues[j] = 0;
            killerperimeterxvalues[j] = 0;
            killerperimeteryvalues[j] = 0;
        }

        int numb = updateperimeters(sites, perimeterxvalues, perimeteryvalues);
        printf("Current number of perimeters: %d\n", numb);
        int killernumb = updateperimeters(killer, killerperimeterxvalues, killerperimeteryvalues);
        printf("Number of perimeters for killer: %d\n", killernumb);


        // make a random number from 0 to numb - 1
        int whichperimeter = pseudorandom(numb);
        printf("Random number from 0 to (%d - 1) was %d \n", numb, whichperimeter);
        printf("That means we select the point %d %d \n", perimeterxvalues[whichperimeter], perimeteryvalues[whichperimeter]);
        int changingxvalue = perimeterxvalues[whichperimeter];
        int changingyvalue = perimeteryvalues[whichperimeter];
        sites[changingxvalue][changingyvalue] = 1;
        printf("I just made %d %d a filled data point \n", changingxvalue, changingyvalue);

        // do the same thing for the killer
        int whichkillerperimeter = pseudorandom(killernumb);
        printf("KILLER: Random number from 0 to (%d - 1) was %d \n", killernumb, whichkillerperimeter);
        printf("That means we select the point %d %d \n", killerperimeterxvalues[whichkillerperimeter], killerperimeteryvalues[whichkillerperimeter]);
        int killerchangingxvalue = killerperimeterxvalues[whichkillerperimeter];
        int killerchangingyvalue = killerperimeteryvalues[whichkillerperimeter];
        killer[killerchangingxvalue][killerchangingyvalue] = 1;
        printf("KILLER: I just made %d %d a filled data point \n", killerchangingxvalue, killerchangingyvalue);
    }

    for (i = 0; i < EDEN_MAX; i++ ) {
        for (j = 0; j < EDEN_MAX; j++ ) {
            if (sites[i][j] == 1) {
                output(eden_out, (i-middle), (j-middle));
                printf("outputting: %d %d \n", i, j);
            }
            if (killer[i][j] == 1) {
                output(killer_out, (i-middle), (j-middle));
            }
        }
    }
    return 0;
}

void output(FILE *fp_out, int x, int y) {
    fprintf(fp_out, "%d\t%d\n", x, y);
    return;
}

int pseudorandom(int modulus) {
    int r = rand() % modulus;
    return r;
}

int updateperimeters(int sites[][EDEN_MAX], int perimeterxvalues[], int perimeteryvalues[]) {
    int n = 0;
    int i, j;
    // New Structure:

    for (i = 1; i < (EDEN_MAX-1); i++) {
        for (j = 1; j < (EDEN_MAX-1); j++) {
            if (  ( (sites[i][j] == 0) || (sites[i][j] == 2) ) && ( (sites[i][j-1] == 1) || (sites[i][j+1] == 1) || (sites[i-1][j] == 1) || (sites[i+1][j] == 1) ) ) {
                sites[i][j] = 2;
                perimeterxvalues[n] = i;
                perimeteryvalues[n] = j;
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
