#include <stdio.h>
#include <omp.h>

long long num_steps = 1000000000;
double step;

int main(int argc, char* argv[])
{
    double x, pi, sum = 0.0;
    int i;
    double start, stop;

    step = 1.0 / (double)num_steps;
    start = omp_get_wtime();

    #pragma omp parallel for private(x) reduction(+:sum)
    for (i = 0; i < num_steps; i++)
    {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    pi = sum * step;
    stop = omp_get_wtime();

    printf("The value of PI is %15.12f\n", pi);
    printf("The time to calculate PI was %f seconds\n", stop - start);

    return 0;
}
