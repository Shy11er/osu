#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
int padding = 10;
using namespace std;

vector<vector<double>> multiplyMatrix(vector<vector<double>>& matrix1, vector<vector<double>>& matrix2) {
	int rows = matrix1.size();
	int cols = matrix2[0].size();
	vector<vector<double>> result(rows, vector<double>(cols, 0));
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < matrix1[0].size(); ++k) {
				result[i][j] += matrix1[i][k] * matrix2[k][j];
				//cout << matrix1[i][k] << " * " << matrix2[k][j]<<endl;
			}
		}
	}
	return result;
}
double vectorNorma(vector<double>y)
{
	double sum = 0;
	for (auto now : y)
		sum += pow(now, 2);
	return sqrt(sum);
}
double vectorNormaMaximum(vector<double>λ)
{
	double maxλ = 0;
	for (auto el : λ)
		if (fabs(el) > fabs(maxλ))
			maxλ = el;
	return fabs(maxλ);
}

vector<double> multMatrixVector(vector<vector<double>>A, vector<double>y0)
{
	vector<double>y1;
	for (int i = 0; i < A.size(); i++)
	{
		double yi = 0;
		//	cout << "y["<<i<<"] = ";
		for (int j = 0; j < A.size(); j++)
		{
			yi += A[i][j] * y0[j];
			/*cout << A[i][j] << " * " << y0[j];
			if (j < A.size() - 1)
				cout << " + ";*/
		}
		//cout<<yi << endl;
		y1.push_back(yi);
	}
	return y1;
}
vector<double> minVectors(vector<double>lambda_k, vector<double>lambda_k_1)
{
	for (int i = 0; i < lambda_k.size(); i++)
		lambda_k[i] -= lambda_k_1[i];
	return lambda_k;
}

void testAnswer(vector<vector<double>>A, double maxλ)
{
	vector<vector<long double>>C1 = { {0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0} };
	//maxλ = norma(λ);
	/*for (auto el : λ)
		maxλ += el;
	maxλ /= λ.size();*/

	//A-lambdaE
	for (int i = 0; i < A.size(); i++)
		A[i][i] -= maxλ;


	//det(A-lambdaE)
	vector<vector<double>>C = A;
	double res = 1;

	int n = A.size();
	for (int k = 0; k < n; k++)
	{

		for (int j = k; j < n; j++)
		{
			C[k][j] = A[k][j] / A[k][k];
			C1[k][j] = A[k][j];
		}

		for (int i = k + 1; i < n; i++)
		{
			for (int j = k + 1; j < n; j++)
				A[i][j] = A[i][j] - C[k][j] * A[i][k];
		}
	}

	for (int i = 0; i < C1.size(); i++)
		res *= C1[i][i];
	cout << "det(A-λE) = " << setprecision(12) << res << endl;
}

void power_law(vector<vector<double>>A, vector<double>y0, double e, bool n)
{
	vector<vector<double>>x;
	vector<vector<double>>y;
	vector<vector<double>>λ;
	vector<double>el = { 0,0,0,0,0 };

	//1)
	y.push_back(y0);

	//2)
	x.push_back(el);
	double norma_y = vectorNorma(y[0]);//0
	for (int i = 0; i < A.size(); i++)
		x[0][i] = (y[0][i] / norma_y);//0

	//3)
	y.push_back(el);
	y[1] = multMatrixVector(A, x[0]);//1

	//4)
	λ.push_back(el);
	λ.push_back(el);
	for (int i = 0; i < A.size(); i++)
		λ[1][i] = (y[1][i] / x[0][i]);

	//5)
	int k = 1;
	do {
		k++;
		//λ_prev = λ;//1
		//x_prev = x;//0
		//y_prev = y;//1
		//6)
		x.push_back(el);
		norma_y = vectorNorma(y[k - 1]);
		for (int i = 0; i < A.size(); i++)
			x[k - 1][i] = y[k - 1][i] / norma_y;//1
		//7)
		y.push_back(el);
		y[k] = multMatrixVector(A, x[k - 1]);//2
		//8)
		λ.push_back(el);
		for (int i = 0; i < A.size(); i++)
			λ[k][i] = y[k][i] / x[k - 1][i];//2

		double min = 0;
		for (int i = 0; i < A.size(); i++)
			if (fabs(λ[k][i] - λ[k - 1][i]) > min)
				min = fabs(λ[k][i] - λ[k - 1][i]);
		if (min <= e)
			break;
	} while (true);


	double maxλ = 0;
	for (auto el1 : λ[k])
		if (fabs(el1) > maxλ)
			maxλ = el1;
	if (!n)
		maxλ = 1 / maxλ;

	cout << "λ = " << setprecision(12) << maxλ << endl;
	testAnswer(A, maxλ);

	x.push_back(el);
	norma_y = vectorNorma(y[k]);//0
	for (int i = 0; i < A.size(); i++)
		x[k][i] = (y[k][i] / norma_y);//0
	cout << "\nСобственный вектор x:\n\n";
	for (auto col : x[k])
		cout << setw(padding * 2) << col << endl << endl;
	cout << endl;

	cout << "\nКоличество итераций: " << k << endl;
}
void obr1(vector<double>& y, vector<double>b, vector<vector<double>>C, vector<vector<double>>A)
{
	/*cout << endl << endl << "C \n";
	for (auto now : C)
	{
		for (auto now1 : now)
			cout << setw(d) << now1;
		cout << endl << endl;
	}
	cout << endl << endl << "A \n";
	for (auto now : A)
	{
		for (auto now1 : now)
			cout << setw(d) << now1;
		cout << endl << endl;
	}*/
	int n = A.size();
	for (int k = 0; k < n; k++)
	{

		for (int j = k; j < n; j++)
		{
			C[k][j] = A[k][j] / A[k][k];
		}
		y[k] = b[k] / A[k][k];

		for (int i = k + 1; i < n; i++)
		{
			for (int j = k + 1; j < n; j++)
				A[i][j] = A[i][j] - C[k][j] * A[i][k];
			b[i] = b[i] - y[k] * A[i][k];
		}

		//cout << endl << endl;
	}
	//cout << endl << endl<<"C2 \n";
	/*for (auto now : C)
	{
		for (auto now1 : now)
			cout << setw(d) << now1;
		cout << endl << endl;
	}*/
	/*cout << endl << endl << "A \n";
	for (auto now : A)
	{
		for (auto now1 : now)
			cout << setw(d) << now1;
		cout << endl << endl;
	}*/
}
void obr(vector<vector<double>>invC, vector<double>& x1, vector<double>E1)
{
	/*cout << endl << endl << "C \n";
	for (auto now : invC)
	{
		for (auto now1 : now)
			cout << setw(d) << now1;
		cout << endl << endl;
	}*/
	/*cout << endl << endl << "x \n";
	for (auto now1 : x1)
		cout << setw(d) << now1 << endl;
	cout << endl << endl;
	cout << endl << endl << "E \n";
	for (auto now1 : E1)
		cout << setw(d) << now1 << endl;
	cout << endl << endl;*/
	int n = 5;
	x1[n - 1] = E1[n - 1];
	for (int i = n - 2; i > -1; i--)
	{
		double sumCX = 0;
		for (int j = i + 1; j < n; j++)
		{
			sumCX += invC[i][j] * x1[j];
		}
		x1[i] = E1[i] - sumCX;
	}
	cout << endl << endl << "x \n";
	for (auto now : x1)
		cout << setw(padding) << now << endl;
	cout << endl << endl;
	/*cout << endl << endl << "E \n";
	for (auto now : E1)
		cout << setw(d) << now << endl;
	cout << endl << endl;*/
}
void vectorScalarMult(vector<vector<double>>A, vector<double>y0, double e, bool n)
{
	vector<vector<double>>x;
	vector<vector<double>>y;
	vector<double>λ;
	vector<double>el = { 0,0,0,0,0 };

	//1)
	y.push_back(y0);

	//2)
	x.push_back(el);
	double norma_y = vectorNorma(y[0]);//0
	for (int i = 0; i < A.size(); i++)
		x[0][i] = (y[0][i] / norma_y);//0

	//3)
	y.push_back(el);
	y[1] = multMatrixVector(A, x[0]);//1

	//4)
	λ.push_back(0);
	λ.push_back(0);
	double sum = 0;
	for (int i = 0; i < A.size(); i++)
	{
		λ[1] += (y[1][i] * y[1][i]);
		sum += (y[1][i] * x[0][i]);
	}
	λ[1] = λ[1] / sum;

	//5)
	int k = 1;


	do {
		k++;
		//λ_prev = λ;//1
		//x_prev = x;//0
		//y_prev = y;//1
		//6)
		x.push_back(el);
		norma_y = vectorNorma(y[k - 1]);
		for (int i = 0; i < A.size(); i++)
			x[k - 1][i] = y[k - 1][i] / norma_y;//1
		//7)
		y.push_back(el);
		y[k] = multMatrixVector(A, x[k - 1]);//2
		//8)
		λ.push_back(0);
		double sum = 0;
		for (int i = 0; i < A.size(); i++)
		{
			λ[k] += (y[k][i] * y[k][i]);
			sum += (y[k][i] * x[k - 1][i]);
		}
		λ[k] = λ[k] / sum;


		if (fabs(λ[k] - λ[k - 1]) <= e)
			break;
		/*if (normamax(minVectors(x[k-2],x[k-1])) <= e)
			break;*/
	} while (true);


	testAnswer(A, λ[k]);
	if (!n)
		λ[k] = 1 / λ[k];
	cout << "λ = " << setprecision(12) << λ[k] << endl;

	x.push_back(el);
	norma_y = vectorNorma(y[k]);//0
	for (int i = 0; i < A.size(); i++)
		x[k][i] = (y[k][i] / norma_y);//0
	cout << "\nСобственный вектор x:\n\n";
	for (auto col : x[k])
		cout << setw(padding * 2) << col << endl << endl;
	cout << endl;

	cout << "\nКоличество итераций: " << k << endl;
}

void algorithmRaley(vector<vector<double>>A, vector<double>y0, double e, bool n)
{
	vector<vector<double>>x;
	vector<vector<double>>y;
	vector<double>λ;
	vector<double>el = { 0,0,0,0,0 };

	//1)
	y.push_back(y0);

	//2)
	x.push_back(el);
	double norma_y = vectorNorma(y[0]);//0
	for (int i = 0; i < A.size(); i++)
		x[0][i] = (y[0][i] / norma_y);//0

	//3)
	y.push_back(el);
	y[1] = multMatrixVector(A, x[0]);//1

	//4)
	λ.push_back(0);
	double sum = 0;
	for (int i = 0; i < A.size(); i++)
	{
		λ[0] += (y[1][i] * x[0][i]);
		sum += (x[0][i] * x[0][i]);
	}
	λ[0] = λ[0] / sum;

	//5)
	int k = 0;


	do {
		k++;
		//λ_prev = λ;//1
		//x_prev = x;//0
		//y_prev = y;//1
		//6)
		x.push_back(el);
		norma_y = vectorNorma(y[k]);
		for (int i = 0; i < A.size(); i++)
			x[k][i] = y[k][i] / norma_y;//1
		//7)
		y.push_back(el);
		y[k + 1] = multMatrixVector(A, x[k]);//2
		//8)
		λ.push_back(0);
		double sum = 0;
		for (int i = 0; i < A.size(); i++)
		{
			λ[k] += (y[k + 1][i] * x[k][i]);
			sum += (x[k][i] * x[k][i]);
		}
		λ[k] = λ[k] / sum;

		if (fabs(λ[k] - λ[k - 1]) <= e)
			break;
	} while (true);



	testAnswer(A, λ[k]);
	if (!n)
		λ[k] = 1 / λ[k];
	cout << "λ = " << setprecision(12) << λ[k] << endl;

	x.push_back(el);
	norma_y = vectorNorma(y[k]);//0
	for (int i = 0; i < A.size(); i++)
		x[k][i] = (y[k][i] / norma_y);//0
	cout << "\nСобственный вектор x:\n\n";
	for (auto col : x[k])
		cout << setw(padding * 2) << col << endl << endl;
	cout << endl;


	cout << "\nКоличество итераций: " << k << endl;
}
int main()
{
	setlocale(0, "rus");
	vector<vector<double>> A = { {14,7.8,0.92,0.23,0.4},{7.8,58.5,3.8,8.48,1.89},{0.92,3.8,9.6,0.28,9.67},{0.23,8.48,0.28,5.85,0.5},{0.4,1.89,9.67,0.5,1.4} };
	vector<double>y = { 1,1,1,1,1 };


	cout << "Матрица А:\n\n";
	for (auto row : A)
	{
		for (auto col : row)
			cout << setw(padding) << col;
		cout << endl << endl;
	}
	cout << "Начальное приближение у:\n\n";
	for (auto col : y)
		cout << setw(padding) << col << endl;
	cout << endl;

	cout << setw(padding) << "Степенной метод.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наибольшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		power_law(A, y, pow(10, -i), true);
		cout << "\n\n";
	}

	cout << setw(padding) << "Метод скалярных произведений.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наибольшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		vectorScalarMult(A, y, pow(10, -i), true);
		cout << "\n\n";
	}
	cout << setw(padding) << "Метод частных Рэлея.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наибольшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		algorithmRaley(A, y, pow(10, -i), true);
		cout << "\n\n";
	}
	//proverka(A,y);



	vector<vector<double>> C = { {0, 0, 0, 0, 0}, { 0,0,0,0,0 }, { 0,0,0,0,0 }, { 0,0,0,0,0 }, { 0,0,0,0,0 } };
	vector<vector<double>> inverseMatrix =
	{ {0.078, -0.013, -0.001, 0.015, -0.002},
		{ -0.013,	0.024, -0.002, -0.034, -0.006 },
		{-0.001, -0.002,	-0.017, -0.007, 0.121,},
		{0.015, -0.034, -0.007,	0.218,	0.013},
		{-0.002, -0.006, 0.121,	0.013,	-0.118} };

	cout << endl << endl << "Обратная матрица A: " << endl;
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = 0; j < A.size(); j++)
			cout << setw(padding * 2) << inverseMatrix[i][j];
		cout << endl << endl;
	}
	cout << endl << endl << "Проверка A * A_1: " << endl;
	vector<vector<double>> result = multiplyMatrix(A, inverseMatrix);
	for (auto now : result)
	{
		for (auto now1 : now)
			cout << setw(padding * 2) << now1;
		cout << endl << endl;
	}

	cout << "Обртаная матрица А:\n\n";
	for (auto row : inverseMatrix)
	{
		for (auto col : row)
			cout << setw(padding) << col;
		cout << endl << endl;
	}
	cout << "Начальное приближение у:\n\n";
	for (auto col : y)
		cout << setw(padding) << col << endl;
	cout << endl;

	cout << setw(padding) << "Степенной метод.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наименьшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		power_law(inverseMatrix, y, pow(10, -i), false);
		cout << "\n\n";
	}

	cout << setw(padding) << "Метод скалярных произведений.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наименьшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		vectorScalarMult(inverseMatrix, y, pow(10, -i), false);
		cout << "\n\n";
	}
	cout << setw(padding) << "Метод частных Рэлея.\n\n";

	for (int i = 3; i <= 9; i++)
	{
		cout << setw(padding) << "Наименьшее собственное значение матрицы A с точностью e = " << pow(10, -i) << ": ";
		algorithmRaley(inverseMatrix, y, pow(10, -i), false);
		cout << "\n\n";
	}
}
