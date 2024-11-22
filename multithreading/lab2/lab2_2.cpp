








#include <bits/stdc++.h>
#include <omp.h>

int main() {
    int x = 0;

    #pragma omp parallel num_threads(4)
    {
      // Only main thread(with number 0) will execute this part
      #pragma omp master
          printf("Master thread: only this thread executes this part.\n");
      

      // Syncrhonize access to the critical section for avoiding changing variables by the several threads
      #pragma omp critical
          x++;
          printf("Thread %d added its number to sum.\n", x);
      
    }

    printf("Final sum = %d\n", x);

    return 0;
}

























