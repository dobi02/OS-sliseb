// pthread_sum.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 1000000
#define THREAD_COUNT 4

int array[SIZE];
long long total_sum = 0;
pthread_mutex_t lock;

typedef struct {
    int start, end;
} Range;

void* partial_sum(void* arg) {
    Range* r = (Range*)arg;
    long long local_sum = 0;

    for (int i = r->start; i < r->end; i++)
        local_sum += array[i];

    pthread_mutex_lock(&lock);
    total_sum += local_sum;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    Range ranges[THREAD_COUNT];

    // 배열 초기화
    for (int i = 0; i < SIZE; i++) array[i] = 1;

    int chunk = SIZE / THREAD_COUNT;
    pthread_mutex_init(&lock, NULL);

    // 스레드 생성
    for (int i = 0; i < THREAD_COUNT; i++) {
        ranges[i].start = i * chunk;
        ranges[i].end = (i == THREAD_COUNT - 1) ? SIZE : (i + 1) * chunk;
        pthread_create(&threads[i], NULL, partial_sum, &ranges[i]);
    }

    // 스레드 종료 대기
    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    pthread_mutex_destroy(&lock);
    printf("Total sum: %lld\n", total_sum);
    return 0;
}
