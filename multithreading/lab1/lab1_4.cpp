#include <bits/stdc++.h>
#include <omp.h>

#define N 10

using namespace std;

int main() {
  // --------------Multi-threaded--------------
  {
    int i;
    double a[N+1], b[N+1], c[N+1], d[N+1];

    for (int i = 0; i < N; i++) {
      a[i] = i * 1.5;
      b[i] = i + 22.35;
    }

    double start = omp_get_wtime();

    #pragma omp parallel num_threads(2) shared(a,b,c,d) private(i) 
    {
      #pragma omp sections nowait
      {
        #pragma omp section
        {
          for (i = 0; i < N; i++) {
            c[i] = a[i] + b[i];
          }

          printf("I'm thread %d. Calculate vectors sum.\n", omp_get_thread_num());
        }

        #pragma omp section 
        {
          for (i = 0; i < N; i++) {
            d[i] = a[i] * b[i];
          }

          printf("I'm thread %d. Calculate vectors sum.\n", omp_get_thread_num());
        }
      }
    }

    double end = omp_get_wtime();

    printf("Time taken in multi-threaded: %lf seconds\n", end - start);
  }

  // --------------Sequential--------------
  
  {
    int i;
    double a[N+1], b[N+1], c[N+1], d[N+1];

    for (i = 0; i < N; i++) {
        a[i] = i * 1.5;
        b[i] = i + 22.35;
    }

    double start = clock();

    for (i = 0; i < N; i++) {
        c[i] = a[i] + b[i];
    }
    printf("Calculate vectors sum.\n");

    for (i = 0; i < N; i++) {
        d[i] = a[i] * b[i];
    }
    printf("Calculate vectors product.\n");

    double end = clock();

    double total_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Time taken in sequential: %lf seconds\n", total_time);

    return 0;
  }
}