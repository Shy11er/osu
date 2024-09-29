#include <bits/stdc++.h>
#include <omp.h>

#define N 1000000
#define CHUNKSIZE 100

int main(void) {
  // WITH PARRALLEL 
  {
    char* a = new char[N];
    char* b = new char[N];
    char* c = new char[N];

    for (int i = 0; i < N; i++) {
      a[i] = b[i] = i;
    }

    int i;
    const double start = omp_get_wtime();
    // #pragma omp parallel for marking parallel region where each thread do same thing
    // private(rank) indicates that the rank variable is local to each thread (local)
    // shared(a,b,c) clause is used to specify variables that are shared among all threads (public/shared)
    // num_threads(4) indicates the number of threads (thread amount)
    #pragma omp parallel num_threads(4) shared(a,b,c) private(i)
    {
      #pragma omp for schedule(dynamic, CHUNKSIZE) nowait
      for (i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
      }
    }
    const double end = omp_get_wtime();
    const double duration = end - start;

    printf("Time taken with parrallel: %lf seconds\n", duration);
    
    delete[] a;
    delete[] b;
    delete[] c;
  }

  // WITHOUT PARRALLEL
  {
    char* a = new char[N];
    char* b = new char[N];
    char* c = new char[N];

    for (int i = 0; i < N; i++) {
      a[i] = b[i] = i;
    }

    const double start = omp_get_wtime();

    for (int i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
    }

    const double end = omp_get_wtime();
    const double duration = end - start;

    printf("Time taken without parrallel: %lf seconds\n", duration);
    
    delete[] a;
    delete[] b;
    delete[] c;
  }

  return 0;
}