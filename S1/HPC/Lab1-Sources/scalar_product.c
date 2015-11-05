/******************************************************************************
 * Perform scalar product of two random vectors.
 * Compilation: gcc -03 scalar_product.c -o sp
 ******************************************************************************/

// Std
#include <stdio.h>
#include <stdlib.h>

// Parsing
#include <unistd.h>

// Time
#include <time.h>

void print_usage(char *s) {
  printf("Usage: %s -n <size of vectors>\n", s);
  exit(0);
}

int main(int argc, char **argv)
{
  // Loop index and size
  int i, size;

  // Arrays
  double *y, *x;

  // Time
  time_t start, stop;

  // Scalar product
  double sp;

  // Check if we have at least one argument
  char c;
  if (argc <=1 ) {
    print_usage(argv[0]);
  }
  else {
    // Parse the arguments for a -h or -n flag
    while ((c=getopt(argc, argv, "hn:")) != EOF) {
      switch (c) {
        case 'h':
          print_usage(argv[0]);
        case 'n':
          size = atoi(optarg);    // Get size of vector
          break;
        default:
          print_usage(argv[0]);
      }
    }
  }
	// Initializes random number generator
	srand((unsigned) time(&start));

  // Allocate arrays
  x = malloc(size*sizeof(double));
  y = malloc(size*sizeof(double));

	// Starting time
	time(&start);
  // Initialize vector
  for(i=0; i<size; i++)
  {
    x[i] = rand();
    y[i] = rand();
  }

  // Perform computation
  sp = 0;

  #pragma omp parallel for schedule(static,100)
  for (i=0; i<size; i++)
  {
    sp += x[i]*y[i];
  }
  // Stopping time
  time(&stop);

  // Elapsed time
  printf("Elapsed time: %lf s\n",(double)(stop-start)/(CLOCKS_PER_SEC)*1000);

  free(x);
  free(y);

  return 0;

}
