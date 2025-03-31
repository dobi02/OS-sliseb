/*
 * Copyright 2022-2023. Heekuck Oh, all rights reserved.
 *
 * 이 프로그램은 한양대학교 ERICA 컴퓨터학부 재학생을 위한 TLS 예제이다.
 * counter와 global은 전역변수이므로 모든 스레드에서 보인다. 즉, 공유한다.
 * 반면에 local은 지역변수이므로 해당 스레드에게만 보인다.
 * 우리는 전역변수 global이 해당 스레드마다 별도의 사본을 가질 수 있게 할 수 없을까?
 * 다시 말해 전역변수 global을 마치 지역변수처럼 해당 스레드만 볼 수 있게 할 수 없을까?
 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 8

int counter = 0;
/*
 * __thread와 _Thread_local은 전역변수를 해당 스레드마다 별도로 가질 수 있게 한다.
 */
int global;
//__thread int global;
//_Thread_local int global;

void *foo(void *arg)
{
    int local;
    
    global = local = counter++;
    printf("%d--created\n", local);
    /*
     * 변수 local과 global 값이 같으면 무한 루프를 돌면서
     * 혹시 자신이 철회 명령을 받았는지 검사한다. (지연철회 여부 검사)
     * 만일 받았다면 pthread_testcancel()에서 리턴되지 않고 철회된다.
     */
    while (local == global)
        pthread_testcancel();
    /*
     * 변수 local과 global 값이 같지 않으면 루프를 벗어나서 아래를 실행한다.
     */
    printf("<%d,%d> %d--finished\n", local, global, local);
    /*
     * 스레드를 정상적으로 종료한다.
     */
    pthread_exit(NULL);
}

int main(void)
{
    int i;
    pthread_t tid[N];
    void *retval;
    
    /*
     * N개의 서브 스레드를 생성한다.
     */
    for (i = 0; i < N; ++i)
        pthread_create(tid+i, NULL, foo, NULL);
    /*
     * 0번 서브 스레드를 분리시킨다.
     * 분리된 스레드는 조인이 불가능하며 조인을 시도하면 오류 값이 리턴된다.
     */
    pthread_detach(tid[0]);
    /*
     * 모든 서브 스레드가 실행되도록 메인 스레드는 잠시 멈춘다.
     */
    sleep(1);
    /*
     * 모든 서브 스레드를 철회한다.
     * 스레드가 이미 종료되었다면 철회는 무시된다.
     */
    for (i = 0; i < N; ++i)
        pthread_cancel(tid[i]);
    /*
     * 모든 서브 스레드를 조인하고, 종료 상태를 출력한다.
     */
    for (i = 0; i < N; ++i) {
        pthread_join(tid[i], &retval);
        if (retval == PTHREAD_CANCELED)
            printf("스레드%d 철회\n", i);
        else if (retval == NULL)
            printf("스레드%d 정상종료\n", i);
        else
            printf("스레드%d 모름\n", i);
    }
    return 0;
}
