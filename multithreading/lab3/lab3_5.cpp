#include <bits/stdc++.h>
#include <omp.h>

int main() {
    printf("Первый вариант: #pragma omp parallel for\n");
    #pragma omp parallel for
    for (int i = 'a'; i <= 'z'; i++) {
        printf("%c ", i);
    }
    
    printf("\n\nВторой вариант: #pragma omp parallel private\n");
    
    int i;
    #pragma omp parallel private(i)
    {
        for (int i = 'a'; i <= 'z'; i++) {
            printf("%c ", i);
        }
    }
    printf("\n\n");

    return 0;
}
