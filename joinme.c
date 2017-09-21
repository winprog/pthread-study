#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define THREAD_NUM  5

void *work(void *t)
{
    int i;
    long tid;
    double result = 0.0;
    tid = (long)t;
    printf("thread %ld starting...\n", tid);
    for (i = 0; i < 10000000; ++i) {
        result += sin(i) * tan(i);
    }
    printf("thread %ld done. reuslt = %e...\n", tid, result);
    pthread_exit((void *)t);
}

int main(int argc, char *argv[])
{
    pthread_t threads[THREAD_NUM];
    long thread_ids[THREAD_NUM];
    pthread_attr_t attr;
    int rc;
    long t;
    void *status;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (t = 0; t < THREAD_NUM; ++t) {
        printf("main: creating thread %ld\n", t);
        thread_ids[t] = t;
        rc = pthread_create(&threads[t], &attr, work, (void *)thread_ids[t]);
        if (rc) {
            printf("error: pthread_create() return %d\n", rc);
            exit(-1);
        }
    }

    pthread_attr_destroy(&attr);
    for (t = 0; t < THREAD_NUM; ++t) {
        rc = pthread_join(threads[t], &status);
        if (rc) {
            printf("error: pthread_join() return %d\n", rc);
            exit(-2);
        }
        printf("main: join thread #%ld with status %ld\n", t, (long)status);
    }
    
    printf("main: exiting\n");
    pthread_exit(NULL);
    return 0;
}
