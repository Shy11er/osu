#include <omp.h>
#include <stdio.h>

int main(void) {
  omp_set_num_threads(8);
  #pragma omp parallel
  {
      int thread_num = omp_get_thread_num();
      int num_threads = omp_get_num_threads();
      printf("Hello, world! I am %d thread from %d threads!\n", thread_num, num_threads);
  }
  return 0;
}