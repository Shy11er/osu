#include <mpi.h> // Библиотека для работы с MPI
#include <stdio.h> // Библиотека для ввода/вывода

/*
Программа демонстрирует использование функции MPI_Bcast для широковещательной передачи данных:
1. Процесс с рангом 0 (источник) заполняет массив данными.
2. С помощью MPI_Bcast массив передаётся всем процессам в MPI-коммуникаторе.
3. Каждый процесс получает свой фрагмент массива и выводит его.
*/

int main(int argc, char** argv) {
    int rank, size;       // Ранг текущего процесса и общее количество процессов
    int data[6];          // Исходный массив данных
    int chunk[2];         // Локальный фрагмент массива для каждого процесса

    // Инициализация MPI
    MPI_Init(&argc, &argv);

    // Получаем ранг текущего процесса и количество процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Проверяем, что число процессов соответствует длине массива
    if (size != 3) {
        if (rank == 0) {
            printf("This program requires exactly 3 processes.\n");
        }
        MPI_Finalize();
        return 0;
    }

    // Процесс с рангом 0 заполняет массив данными
    if (rank == 0) {
        for (int i = 0; i < 6; i++) {
            data[i] = i + 1;
        }
    }

    // Передаём массив data от процесса 0 всем процессам
    MPI_Bcast(data, 6, MPI_INT, 0, MPI_COMM_WORLD);

    // Распределяем элементы массива между процессами
    for (int i = 0; i < 2; i++) {
        chunk[i] = data[rank * 2 + i];
    }

    // Каждый процесс выводит свой фрагмент
    printf("I am process %d. My elements are: ", rank);
    for (int i = 0; i < 2; i++) {
        printf("%d ", chunk[i]);
    }
    printf("\n");

    // Завершаем работу MPI
    MPI_Finalize();

    return 0;
}
