#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>

// Функция: Блочное распределение с omp for
void process_matrix_block(std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    // #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        if (i % 2 == 0) { // Чётные строки: удваиваем элементы
            for (int j = 0; j < cols; j++) {
                matrix[i][j] *= 2;
            }
        } else { // Нечётные строки: переворачиваем строку
            for (int j = 0; j < cols / 2; j++) {
                std::swap(matrix[i][j], matrix[i][cols - j - 1]);
            }
        }
    }
}

// Функция: Циклическое распределение с omp for
void process_matrix_cyclic(std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();

    #pragma omp parallel for schedule(static, 1)
    for (int i = 0; i < rows; i++) {
        if (i % 2 == 0) { // Чётные строки: удваиваем элементы
            for (int j = 0; j < cols; j++) {
                matrix[i][j] *= 2;
            }
        } else { // Нечётные строки: переворачиваем строку
            for (int j = 0; j < cols / 2; j++) {
                std::swap(matrix[i][j], matrix[i][cols - j - 1]);
            }
        }
    }
}

// Функция: Блочное распределение без omp for
void process_matrix_block_manual(std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int num_threads = omp_get_max_threads();

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int block_size = (rows + num_threads - 1) / num_threads;
        int start = thread_id * block_size;
        int end = std::min(start + block_size, rows);

        for (int i = start; i < end; i++) {
            if (i % 2 == 0) { // Чётные строки: удваиваем элементы
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] *= 2;
                }
            } else { // Нечётные строки: переворачиваем строку
                for (int j = 0; j < cols / 2; j++) {
                    std::swap(matrix[i][j], matrix[i][cols - j - 1]);
                }
            }
        }
    }
}

// Функция: Циклическое распределение без omp for
void process_matrix_cyclic_manual(std::vector<std::vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    int num_threads = omp_get_max_threads();

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        for (int i = thread_id; i < rows; i += num_threads) {
            if (i % 2 == 0) { // Чётные строки: удваиваем элементы
                for (int j = 0; j < cols; j++) {
                    matrix[i][j] *= 2;
                }
            } else { // Нечётные строки: переворачиваем строку
                for (int j = 0; j < cols / 2; j++) {
                    std::swap(matrix[i][j], matrix[i][cols - j - 1]);
                }
            }
        }
    }
}

// Замер времени выполнения
double measure_time(void (*func)(std::vector<std::vector<int>>&), std::vector<std::vector<int>>& matrix) {
    double start = omp_get_wtime();
    func(matrix);
    double end = omp_get_wtime();
    return end - start;
}

int main() {
    const int rows = 2000, cols = 2000;
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(cols, 1));
    std::vector<std::vector<int>> temp_matrix = matrix;

    // Замер времени для каждого подхода
    double time_block = measure_time(process_matrix_block, temp_matrix);
    std::cout << "Блочное распределение с omp for: " << time_block << " сек\n";

    temp_matrix = matrix; // Сбрасываем матрицу
    double time_cyclic = measure_time(process_matrix_cyclic, temp_matrix);
    std::cout << "Циклическое распределение с omp for: " << time_cyclic << " сек\n";

    temp_matrix = matrix; // Сбрасываем матрицу
    double time_block_manual = measure_time(process_matrix_block_manual, temp_matrix);
    std::cout << "Блочное распределение без omp for: " << time_block_manual << " сек\n";

    temp_matrix = matrix; // Сбрасываем матрицу
    double time_cyclic_manual = measure_time(process_matrix_cyclic_manual, temp_matrix);
    std::cout << "Циклическое распределение без omp for: " << time_cyclic_manual << " сек\n";

    return 0;
}
