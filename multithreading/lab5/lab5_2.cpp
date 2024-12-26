#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    int N = 8; // Размер массива (по умолчанию)

    if (argc == 2) {
        N = atoi(argv[1]); // Считываем размер массива из аргументов командной строки
    }

    srand(time(NULL)); // Инициализация генератора случайных чисел
    int size, rank;
    int* sendbuf = nullptr; // Массив данных
    int* recvbuf = nullptr; // Буфер для приема данных на процессе 0
    int* local_row = new int[N]; // Буфер для одной строки

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Проверка, что число строк делится на количество процессов
    if (N % size != 0) {
        if (rank == 0) printf("Количество строк массива должно делиться на количество процессов.\n");
        MPI_Finalize();
        return 0;
    }

    // Процесс 0 инициализирует массив
    if (rank == 0) {
        sendbuf = new int[N * N];
        printf("Изначальный массив:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sendbuf[i * N + j] = rand() % 10; // Заполнение случайными числами
                printf("%d ", sendbuf[i * N + j]); // Вывод массива
            }
            printf("\n");
        }
    }

    // Рассылка строк массива всем процессам
    MPI_Scatter(sendbuf, N, MPI_INT, local_row, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Все процессы отправляют свои строки обратно процессу 0 
    // recvbuf на процессе с рангом 0, чтобы он мог принять собранные строки от всех процессов при вызове MPI_Gather
    if (rank == 0) {
        recvbuf = new int[N * N];
    }
    MPI_Gather(local_row, N, MPI_INT, recvbuf, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Процесс 0 выводит собранный массив построчно
    if (rank == 0) {
        printf("\nСобранный массив на процессе 0:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                printf("%d ", recvbuf[i * N + j]);
            }
            printf("\n");
        }
        delete[] recvbuf;
    }

    // Завершение работы MPI
    MPI_Finalize();

    // Освобождение памяти
    if (rank == 0) {
        delete[] sendbuf;
    }
    delete[] local_row;

    return 0;
}
