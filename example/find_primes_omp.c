/*
 * Copyright(c) 2023-2024 All rights reserved by Heekuck Oh.
 * 이 프로그램은 한양대학교 ERICA 컴퓨터학부 학생을 위한 교육용으로 제작되었다.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>
#include <omp.h>

#define N 8
#define STARTNUM 1000000000000000000UL
#define SRCHSPAN 1000UL

/*
 * isprime은 n이 소수이면 참, 아니면 거짓을 리턴한다.
 */
bool isprime(uint64_t n)
{
    uint64_t p = 5;
    
    if (n == 2 || n == 3)
        return true;
    if (n == 1 || n % 2 == 0 || n % 3 == 0)
        return false;
    while (n >= p*p)
        if (n % p == 0 || n % (p + 2) == 0)
            return false;
        else
            p += 6;
    return true;
}

int main(void)
{
    int i;

    /*
     * 소수를 찾는 스레드를 CPU 코어의 수 만큼 생성한다.
     */
    #pragma omp parallel for
    for (i = 0; i < N; ++i) {
        for (uint64_t j = STARTNUM + i * SRCHSPAN; j < STARTNUM + (i+1) * SRCHSPAN; ++j)
            if (isprime(j))
                printf("[%d]%"PRIu64"\n", i, j);
    }
    /*
     * 메인함수를 종료한다.
     */
    return 0;
}
