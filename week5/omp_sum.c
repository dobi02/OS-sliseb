// omp_sum.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 1000000

int array[SIZE];

int main() {
    long long total_sum = 0;

    // 배열 초기화
    for (int i = 0; i < SIZE; i++) array[i] = 1;

    // OpenMP를 이용한 배열 합산
    #pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < SIZE; i++) {
        total_sum += array[i];
    }

    printf("Total sum: %lld\n", total_sum);
    return 0;
}
