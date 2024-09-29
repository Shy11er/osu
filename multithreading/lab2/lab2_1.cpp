#include <bits/stdc++.h>
#include <omp.h>

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
