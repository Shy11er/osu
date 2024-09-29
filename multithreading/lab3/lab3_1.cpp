#include <bits/stdc++.h>
#include <omp.h>

#define BLOCK_SIZE 500

int main() {
  // --------------Sequential--------------
  {
    unsigned int iter = 200000000;
    int i, j;
    double x, y;
    double dUnderCurve = 0.0;
    double pi = 0.0;
    double r[BLOCK_SIZE * 2];
    double end_time, start_time;

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
      r[i] = (float)(rand() % 100) / 100.0;
    }

    start_time = omp_get_wtime();

    for (j = 0; j < iter / BLOCK_SIZE; j++) {
      for (i = 0; i < BLOCK_SIZE; i++) {
        x = r[i];
        y = r[i + BLOCK_SIZE];
        if (x * x + y * y <= 1.0) {
          dUnderCurve++;
        }
      }
    }

    pi = dUnderCurve / (double) iter * 4;
    end_time = omp_get_wtime();
    printf("pi = %10.9f\n", pi);
    printf("time with sequential = %10.9f\n", (double)((end_time - start_time) / 1000.0));
  }

  // --------------Multi-threaded--------------
  
  {
    unsigned int iter = 200000000;
    int i, j;
    double x, y;
    double dUnderCurve = 0.0;
    double pi = 0.0;
    double r[BLOCK_SIZE * 2];
    double end_time, start_time;

    for (i = 0; i < BLOCK_SIZE * 2; i++) {
      r[i] = (float)(rand() % 100) / 100.0;
    }

    start_time = omp_get_wtime();

    #pragma omp parallel for private(i, x, y) reduction(+:dUnderCurve)
    for (j = 0; j < iter / BLOCK_SIZE; j++) {
      for (i = 0; i < BLOCK_SIZE; i++) {
        x = r[i];
        y = r[i + BLOCK_SIZE];
        if (x * x + y * y <= 1.0) {
          dUnderCurve++;
        }
      }
    }

    pi = dUnderCurve / (double) iter * 4;
    end_time = omp_get_wtime();
    printf("pi = %10.9f\n", pi);
    printf("time with multi-threading = %10.9f\n", (double)((end_time - start_time) / 1000.0));
  }
}
