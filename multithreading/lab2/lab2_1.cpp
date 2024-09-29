#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

static long num_steps = 10000;
double step;

int main() {
  // --------------Sequential--------------
  {
    int i;
    double x, pi, sum = 0.0;
    double start_time = omp_get_wtime();

    step = 1.0 / (double) num_steps;

    for (i = 0; i < num_steps; i++) {
      x = (i + 0.5) * step;
      sum = sum + 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Calculated pi = %f\n", pi);
    printf("Sequential execution time: %f seconds\n", end_time - start_time);
  }

  // --------------Multi-threaded--------------
  {
    int i;
    double x, pi, sum = 0.0;
    double start_time = omp_get_wtime();

    step = 1.0 / (double) num_steps;

    // reduction(+:sum) creates local copies of the sum variable for each thread and also sums the values when parallel region is ends
    #pragma omp parallel for private(x) reduction(+:sum)
    for (i = 0; i < num_steps; i++) {
      x = (i + 0.5) * step;
      sum += 4.0 / (1.0 + x * x);
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Calculated pi = %f\n", pi);
    printf("Parallel execution with reduction took: %f seconds\n", end_time - start_time);

  }
}