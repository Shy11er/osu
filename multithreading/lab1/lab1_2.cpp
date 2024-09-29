#include <bits/stdc++.h>
#include <omp.h>

int main(void) {
  int rank;
  
  // #pragma omp parallel for marking parallel region where each thread do same thing
  // private(rank) indicates that the rank variable is local to each thread
  // num_threads(3) indicates the number of threads
  #pragma omp parallel private(rank) num_threads(3)
  {
    rank = omp_get_thread_num();
    printf("Current rank is %d\n", rank);
    if (rank == 0) {
      printf("This message only for private variables.\n");
    }
  }

  return 0;
}