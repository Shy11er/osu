#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
Каждому процессу передаётся равное количество строк. Например, при 𝑁 =8
N=8 и 
size
=
4
size=4 (4 процесса), каждый процесс получает 2 строки.
*/

// Функция обработки строки
void process_row(int* row, int N, int is_odd) {
    if (is_odd) {
        // Если номер строки нечетный, переворачиваем строку справа налево
        for (int i = 0; i < N / 2; i++) {
            int temp = row[i];
            row[i] = row[N - i - 1];
            row[N - i - 1] = temp;
        }
    } else {
        // Если номер строки четный, удваиваем каждый элемент строки
        for (int i = 0; i < N; i++) {
            row[i] *= 2;
        }
    }
}

int main(int argc, char** argv) {
    int N = 8; // Размер матрицы по умолчанию (8x8)

    // Если передан аргумент, используем его как размер матрицы
    if (argc == 2) {
        N = atoi(argv[1]);
    }

    // Инициализация переменных
    srand(time(NULL)); // Инициализация генератора случайных чисел
    int size, rank, sendcount, recvcount, source;
    int* sendbuf;

    MPI_Init(&argc, &argv); // Инициализация MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Получаем текущий ранг процесса
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Получаем общее количество процессов

    // Проверяем, делится ли количество строк на число процессов
    if (N % size) {
        if (rank == 0) printf("Количество строк не делится на число процессов.\n");
        MPI_Finalize();
        return 0;
    }

    // Выделяем память для исходной матрицы (только в процессе с рангом 0)
    sendbuf = new int[N * N];

    if (rank == 0) {
        // Инициализация матрицы и вывод на экран
        printf("Исходная матрица:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sendbuf[i * N + j] = rand() % 10; // Генерация случайных чисел от 0 до 9
                printf("%d ", sendbuf[i * N + j]); // Вывод элемента
            }
            printf("\n");
        }
    }

    // Число строк, обрабатываемых каждым процессом
    int rows_per_proc = N / size;
    // Буфер для строк, которые обрабатывает текущий процесс
    int* recvbuf = new int[rows_per_proc * N];
    source = 0; // Процесс-источник данных (обычно процесс с рангом 0)

    // Распределяем строки матрицы между процессами
    MPI_Scatter(sendbuf, rows_per_proc * N, MPI_INT, recvbuf, rows_per_proc * N, MPI_INT, source, MPI_COMM_WORLD);

    // Обрабатываем строки, которые были распределены этому процессу
    for (int i = 0; i < rows_per_proc; i++) {
        // Определяем, четная или нечетная строка, и обрабатываем её
        process_row(&recvbuf[i * N], N, (rank * rows_per_proc + i) % 2 != 0);
    }

    // Собираем обработанные строки обратно в исходную матрицу
    MPI_Gather(recvbuf, rows_per_proc * N, MPI_INT, sendbuf, rows_per_proc * N, MPI_INT, source, MPI_COMM_WORLD);

    if (rank == 0) {
        // Вывод обработанной матрицы
        printf("Обработанная матрица:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", sendbuf[i * N + j]);
            }
            printf("\n");
        }
    }

    // Освобождаем выделенную память
    delete[] sendbuf;
    delete[] recvbuf;
    MPI_Finalize(); // Завершаем работу MPI
    return 0;
}
