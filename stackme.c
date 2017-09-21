#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_NUM  4
#define N   1000
#define MEGEXTRA    10000

pthread_attr_t  attr;

void *work(void *p)
{
    double A[N][N];
    int i, j;
    long tid = *(long*)p;
    size_t stack_size;

    pthread_attr_getstacksize(&attr, &stack_size);
    printf("thread %ld: stack size = %li bytes\n", tid, stack_size);
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            A[i][j] = ((i*j)/3.452) + (N-i);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[THREAD_NUM];
    long thread_ids[THREAD_NUM];
    size_t stack_size;
    int rc;
    long t;

    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stack_size);
    printf("default stack size = %li\n", stack_size);

    stack_size = sizeof(double)*N*N + MEGEXTRA;
    printf("stack size needed = %li\n", stack_size);
    pthread_attr_setstacksize(&attr, stack_size);
    for (t = 0; t < THREAD_NUM; ++t) {
        thread_ids[t] = t;
        rc = pthread_create(&threads[t], &attr, work, &thread_ids[t]);
        if (rc) {
            printf("error: pthread_create() return %d\n", rc);
            exit(-1);
        }
    }
    printf("created %ld threads.\n", t);
    pthread_exit(NULL);
    return 0;
}

