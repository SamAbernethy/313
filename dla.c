#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

/* PSEUDO CODE
 * Choose a semi-random location for a random walk to start
 * Carry out random walk until walker reaches a perimeter site
 * Also stop walk if the particle gets too far from the cluster
 * "Too far": 1.5 times r_initial plus cluster radius
 * Change the perimeter site into an occupied site and surrounding
 * empty sites into perimeter sites
 * Repeat for a given number of walkers
 * */

#define NUM_WALKERS 5000
#define GRID_SIZE 1401

int pseudorandom(int modulus);
void setupgrid(int sites[][GRID_SIZE], int middle);
void walkstep(int *x, int *y);
void updategrid(int *x, int *y, int sites[][GRID_SIZE]);
void output(FILE *fp_out, int sites[][GRID_SIZE], int middle);

int main(void) {
    FILE *fp_out, *fp_angle;
    fp_out = fopen("dla.dat", "w");
    fp_angle = fopen("angle.dat", "w");

    /*Set up initial grid*/
    int middle = (GRID_SIZE - 1) / 2; // GRID_SIZE must be odd
    int sites[GRID_SIZE][GRID_SIZE];
    setupgrid(sites, middle);

    int walker;
    int x; // current x position of walker
    int y; // current y position of walker
    double r; // current distance of walker from middle of grid
    double theta; // angle from middle of grid to start of walk
    double max_r; // stop walk if walker gets this far from middle
    double r_cluster = 0; // Max distance of cluster from middle

    srand((unsigned)time(NULL));
    for (walker = 0; walker < NUM_WALKERS; walker++) {
        // Pick start point for random walk
        printf("\nWalker number %d\n", walker+1);
	printf("Current cluster radius: %lf\n",r_cluster);
	r = 1.5 * r_cluster + 20;
	max_r = 2 * r_cluster + 40;
	if (max_r > GRID_SIZE / 2) {
	  printf("Too large for grid. Terminating now.\n");
	  return 1;
	}
        theta = 2.0 * M_PI * (double)pseudorandom(1001) / 1000.0;
	fprintf(fp_angle,"%lf\n",theta);
	x = middle + (int)(r * cos(theta));
	y = middle + (int)(r * sin(theta));
	printf("Theta = %lf\n",theta);
	printf("Started walk at %d, %d\n",x,y);

        // Walk until too far away or perimeter site reached
        while (r < max_r) {
            walkstep(&x,&y);
	    //	    printf("%d\t%d\n",x,y);
	    r = sqrt((double)(pow(x - middle,2)+pow(y - middle,2)));
            // Check if we are on a perimeter site
	    if (sites[x][y] == 2) {
	        printf("Hit a perimeter site\n");
	        updategrid(&x,&y,sites);
		if (r > r_cluster) {r_cluster = r;}
		break;
	    }
	}
    }

    output(fp_out, sites, middle);
    fclose(fp_out);
    return 0;
}

/*Places seed at origin, perimeters sites around it and zeros elsewhere*/
void setupgrid(int sites[][GRID_SIZE], int middle) {
    int i, j;
    for (i = 0; i < GRID_SIZE; i++ ) {
        for (j = 0; j < GRID_SIZE; j++ ) {
            sites[i][j] = 0;
        }
    }
    sites[middle][middle] = 1;
    sites[middle+1][middle] = 2;
    sites[middle-1][middle] = 2;
    sites[middle][middle+1] = 2;
    sites[middle][middle-1] = 2;
    printf("Seed located at %d, %d\n",middle,middle);
}

/*Generates a pseudo random number between 0 and (modulus - 1)*/
int pseudorandom(int modulus) {
    int r = rand() % modulus;
    return r;
}

/*Causes the walker to move one unit left, right, up or down*/
void walkstep(int *x, int *y) {
    // Make this part into a "walkstep" function
  //    srand((unsigned)time(NULL));
    int j = pseudorandom(4);
    //printf("Random integer is %d\n", j);
    if (j == 0) {
      *x += 1;
    }
    else if (j == 1) {
      *x -= 1;
    }
    else if (j == 2) {
      *y += 1;
    }
    else {
      *y -= 1;
    }
}

void updategrid(int *x, int *y, int sites[][GRID_SIZE]) {
    sites[*x][*y] = 1;
    printf("Placed element at site %d, %d\n",*x,*y);
    if (sites[*x+1][*y] == 0) {
      sites[*x+1][*y] = 2;
    }
    if (sites[*x-1][*y] == 0) {
      sites[*x-1][*y] = 2;
    }
    if (sites[*x][*y+1] == 0) {
      sites[*x][*y+1] = 2;
    }
    if (sites[*x][*y-1] == 0) {
      sites[*x][*y-1] = 2;
    }
}

void output(FILE *fp_out, int sites[][GRID_SIZE], int middle) {
    int i, j, x, y;
    for (i = 0; i < GRID_SIZE; i++ ) {
        for (j = 0; j < GRID_SIZE; j++ ) {
            if (sites[i][j] == 1) {
	      x = i - middle;
	      y = j - middle;
	      fprintf(fp_out, "%d\t%d\n", x, y);
            }
        }
    }
}
