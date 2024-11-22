#include <stdio.h>
#include <math.h>
#include <omp.h>

int main() {
    int i, j, limit, start, end;
    int number_of_primes = 0;
    int number_of_41primes = 0;
    int number_of_43primes = 0;
    int prime;
    double end_time, start_time;

    start = 200000000;
    end = 300000000;
    if (!(start % 2)) start++;

    printf("Диапазон проверки простых чисел: %d - %d\n\n", start, end);
    start_time = omp_get_wtime();

    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        {
            num_threads = omp_get_num_threads();
            printf("Количество потоков: %d\n", num_threads);
        }
    }

    int *thread_workload = (int *)calloc(num_threads, sizeof(int));
    int *thread_primes_found = (int *)calloc(num_threads, sizeof(int));

    #pragma omp parallel private(i, j, limit, prime)
    {
        int thread_id = omp_get_thread_num();
        int local_workload = 0;
        int local_primes_found = 0;

        #pragma omp for  reduction(+:number_of_primes, number_of_41primes, number_of_43primes)
        for (i = start; i <= end; i += 2) {
            limit = (int) sqrt((float) i) + 1;
            prime = 1;
            j = 3;

            while (prime && (j <= limit)) {
                if (i % j == 0) prime = 0;
                j += 2;
            }

            local_workload++;

            if (prime) {
                local_primes_found++; 
                number_of_primes++;
                if (i % 4 == 1) number_of_41primes++;
                if (i % 4 == 3) number_of_43primes++;
            }
        }

        thread_workload[thread_id] = local_workload;
        thread_primes_found[thread_id] = local_primes_found;
    }

    end_time = omp_get_wtime();

    printf("\nОбщее время работы = %f секунд\n", end_time - start_time);
    printf("Найдено %d простых чисел\n", number_of_primes);
    printf("Количество простых чисел вида 4n+1: %d\n", number_of_41primes);
    printf("Количество простых чисел вида 4n-1: %d\n", number_of_43primes);

    for (i = 0; i < num_threads; i++) {
        printf("\nПоток %d обработал %d чисел, нашел %d простых чисел.\n", i, thread_workload[i], thread_primes_found[i]);
    }

    free(thread_workload);
    free(thread_primes_found);

    return 0;
}
