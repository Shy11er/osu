#include <bits/stdc++.h>
#include <omp.h>

static long num_steps = 100000;
double step;

int main() {
    int i;
    double x, pi, sum = 0.0;
    double start_time = omp_get_wtime();

    step = 1.0 / (double) num_steps;

    #pragma omp parallel
    {
        double sum_local = 0.0;

        #pragma omp for private(x)
        for (i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum_local = sum_local + 4.0 / (1.0 + x * x);
        }

        #pragma omp critical
        {
            sum += sum_local;
        }
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Calculated pi = %f\n", pi);
    printf("Parallel execution time without reduction: %f seconds\n", end_time - start_time);

    return 0;
}
