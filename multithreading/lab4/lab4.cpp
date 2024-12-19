#include <iostream>
#include<vector>
#include<cmath>
#include<omp.h>
#include<time.h>
#include<iomanip>


using namespace std;
int d = 15;
int D = 5;
int N = 1000;

void parallel1( vector<vector<int>>matrix)
{
	int maxlength = 0;
	int length = 0;
	int colnum = 0;
	clock_t start, stop;
	start = clock();
#pragma omp parallel for private(length) shared(colnum,maxlength)
	for (int j = 0; j < N; j++)
	{
		length = 1;
		for (int i = 1; i < N; i++)
		{
			if (matrix[i][j] == matrix[i - 1][j])
			{
				length++;
			}
			else
			{
				length = 1;
			}
#pragma omp critical
			if (length > maxlength)
			{
				colnum = j;
				maxlength = length;
			}
		}
	}

	stop = clock();
	colnum++;
	printf("1) ");
	printf("Время: %f\n\n", ((double)(stop - start) / 1000.0));
	printf("Самая длинная серия одинаковых элементов длинной %d находится в столбце %d\n\n", maxlength, colnum);
}

void parallel2(vector<vector<int>>matrix)
{
	int maxlength = 0;
	int length = 0;
	int colnum = 0;
	int i, j;
	int num_threads = 4;
	int block = (N) / (num_threads);
	int begin, end;
	clock_t start, stop;
	start = clock();
#pragma omp parallel num_threads(num_threads) private(length, begin, end,i,j) shared(maxlength)
	{
		begin = omp_get_thread_num() * block;
		end = (omp_get_thread_num() + 1) * (block)-1;
		//printf("Начало: %d; Конец: %d;\n",begin, end);
		for (j = begin; j <= end; j++)
		{
			length = 1;
			for (i = 1; i < N; i++)
			{
				//printf("Столбец: %d; Строка: %d; Длина: %d; Поток: %d; \n", j, i, length, omp_get_thread_num());
				if (matrix[i][j] == matrix[i - 1][j])
				{
					length++;
				}
				else
				{
					length = 1;
				}
				//printf("Текущая длина: %d; Текущая максимальная длина %d; Текущий поток: %d;\n\n", length, maxlength, omp_get_thread_num());
#pragma omp critical
				{
					if (length > maxlength)
					{
						colnum = j;
						maxlength = length;
					}
				}
			}
		}
	}

	stop = clock();
	colnum++;
	printf("2) ");
	printf("Время: %f\n\n", ((double)(stop - start) / 1000.0));
	printf("Самая длинная серия одинаковых элементов длинной %d находится в столбце %d\n\n", maxlength, colnum);
}

void parallel3(vector<vector<int>>matrix)
{
	
	int maxlength = 0;
	int length = 0;
	int colnum = 0;
	int num_threads = 4;
	int block = (N) / (num_threads);
	clock_t start, stop;
	start = clock();
#pragma omp parallel for schedule(dynamic, block) private(length) shared(colnum,maxlength)
	for (int j = 0; j < N; j++)
	{
		length = 1;
		for (int i = 1; i < N; i++)
		{
			if (matrix[i][j] == matrix[i - 1][j])
			{
				length++;
			}
			else
			{
				length = 1;
			}
#pragma omp critical
			if (length > maxlength)
			{
				colnum = j;
				maxlength = length;
			}
		}
	}


	stop = clock();
	colnum++;
	printf("3) ");
	printf("Время: %f\n\n", ((double)(stop - start) / 1000.0));
	printf("Самая длинная серия одинаковых элементов длинной %d находится в столбце %d\n\n", maxlength, colnum);
}

void parallel4(vector<vector<int>>matrix)
{
	int maxlength = 0;
	int length = 0;
	int colnum = 0;
	int begin, step;
	clock_t start, stop;
	start = clock();
#pragma omp parallel private(length) shared(colnum,maxlength)
	{
		begin = omp_get_thread_num();
		step = omp_get_num_threads();
		for (int j = begin; j < N; j+=step)
		{
			length = 1;
			for (int i = 1; i < N; i++)
			{
				if (matrix[i][j] == matrix[i - 1][j])
				{
					length++;
				}
				else
				{
					length = 1;
				}
#pragma omp critical
				if (length > maxlength)
				{
					colnum = j;
					maxlength = length;
				}
			}
		}
	}
	stop = clock();
	colnum++;
	printf("4) ");
	printf("Время: %f\n\n", ((double)(stop - start) / 1000.0));
	printf("Самая длинная серия одинаковых элементов длинной %d находится в столбце %d\n\n", maxlength, colnum);
}

void iter()
{
	srand(time(0));
	vector<vector<int>>matrix;
	for (int i = 0; i < N; i++)
	{
		vector<int>row;
		matrix.push_back(row);
		for (int j = 0; j < N; j++)
			matrix[i].push_back((rand() % D));
	}

	//cout << "Матрица:" << endl;
	//for (auto now : matrix)
	//{
	//	for (auto now1 : now)
	//		cout << setw(d) << now1;
	//	cout << endl << endl;
	//}
	//cout << endl << endl;

	parallel1(matrix);
	parallel2(matrix);
	parallel3(matrix);
	parallel4(matrix);
}

int main()
{
	setlocale(0, "rus");
	
	iter();


}