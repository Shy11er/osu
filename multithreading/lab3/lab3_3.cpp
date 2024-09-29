#include <bits/stdc++.h>
#include <omp.h>

int main() {
    int i, j, limit, start, end;
    int number_of_primes = 0;
    int number_of_41primes = 0;
    int number_of_43primes = 0;
    int prime;
    double end_time, start_time;

    start = 1;
    end = 10000000;
    if (!(start % 2)) start++;

    printf("Range to check for Primes: %d - %d\n\n", start, end);
    start_time = omp_get_wtime();

    #pragma omp parallel private(i, j, limit, prime)
    {
        #pragma omp for reduction(+:number_of_primes, number_of_41primes, number_of_43primes)
        for (i = start; i <= end; i += 2) {
            limit = (int) sqrt((float) i) + 1;
            prime = 1;
            j = 3;

            while (prime && (j <= limit)) {
                if (i % j == 0) prime = 0;
                j += 2;
            }

            if (prime) {
                number_of_primes++;
                if (i % 4 == 1) number_of_41primes++;
                if (i % 4 == 3) number_of_43primes++;
            }
        }
    }

    end_time = omp_get_wtime();
    printf("Total work time = %f seconds\n", end_time - start_time);
    printf("%d primes found\n", number_of_primes);
    printf("Number of 4n+1 primes found: %d\n", number_of_41primes);
    printf("Number of 4n-1 primes found: %d\n", number_of_43primes);

    return 0;
}
