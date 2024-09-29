#include <bits/stdc++.h>
#include <omp.h>

#define N 1000

void matrix_multiply(double** A, double** B, double** C) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      C[i][j] = 0;
      for (int k = 0; k < N; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void parallel_matrix_multiply(double** A, double** B, double** C) {
  #pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      C[i][j] = 0;
      for (int k = 0; k < N; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

int main() {
  double** A = new double*[N];
  double** B = new double*[N];
  double** C = new double*[N];
  for (int i = 0; i < N; i++) {
      A[i] = new double[N];
      B[i] = new double[N];
      C[i] = new double[N];
  }

  {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i][j] = i + j;
        B[i][j] = i - j;
      }
    }

    double start = omp_get_wtime();
    matrix_multiply(A, B, C);
    double end = omp_get_wtime();

    printf("Time for sequential: %f seconds\n", end - start);
  }

  {
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        A[i][j] = i + j;
        B[i][j] = i - j;
      }
    }

    double start = omp_get_wtime();
    parallel_matrix_multiply(A, B, C);
    double end = omp_get_wtime();

    printf("Time for parallel: %f seconds\n", end - start);
  }

  for (int i = 0; i < N; i++) {
    delete[] A[i];
    delete[] B[i];
    delete[] C[i];
  }
  
  delete[] A;
  delete[] B;
  delete[] C;
}
