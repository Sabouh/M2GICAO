/******************************************************************************
 * Display core id and number of core.
 * Compilation: gcc -fopenmp -03 display_omp.c -o display_omp
 ******************************************************************************/

// Std
#include <stdio.h>

// OpenMP
#include <omp.h>

int main()
{
// Define parallel region
#pragma omp parallel
  {
  // Print relevant informations
    printf("thread : %d sur %d \n" , omp_get_thread_num(),
        omp_get_num_threads());
  }
  return 0;
}
