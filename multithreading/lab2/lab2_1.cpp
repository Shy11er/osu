#include <bits/stdc++.h>
#include <omp.h>

<<<<<<< HEAD
<<<<<<< HEAD
long long num_steps = 1000000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	double x, pi, sum = 0.0;
	int i;

	step = 1.0 / (double)num_steps;
	start =  omp_get_wtime();

	#pragma omp parallel
	{
		double local_sum = 0.0;
		#pragma omp for
		for (i = 0; i < num_steps; i++)
		{
			x = (i + 0.5) * step;
			local_sum += 4.0 / (1.0 + x * x);
		}
		
		#pragma omp critical
		{
			sum += local_sum;
		}
	}

	pi = sum * step;
	stop =  omp_get_wtime();

	printf("The value of PI is %15.12f\n", pi);
	printf("The time to calculate PI was %ld seconds\n", stop - start);
}
=======
using namespace std;

static long num_steps = 10000;
=======
long long num_steps = 1000000000;
>>>>>>> 4ee5634 (fix)
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	double x, pi, sum = 0.0;
	int i;

	step = 1.0 / (double)num_steps;
	start =  omp_get_wtime();

	#pragma omp parallel
	{
		double local_sum = 0.0;
		#pragma omp for
		for (i = 0; i < num_steps; i++)
		{
			x = (i + 0.5) * step;
			local_sum += 4.0 / (1.0 + x * x);
		}
		
		#pragma omp critical
		{
			sum += local_sum;
		}
	}

	pi = sum * step;
	stop =  omp_get_wtime();

<<<<<<< HEAD
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
>>>>>>> 995bc7f (lab2)
=======
	printf("The value of PI is %15.12f\n", pi);
	printf("The time to calculate PI was %ld seconds\n", stop - start);
}
>>>>>>> 4ee5634 (fix)
