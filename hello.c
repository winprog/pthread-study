#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREAD_NUM 5

void *print_hello(void *tid)
{
    long thread_id = *(long *) tid;
    printf("hellow, world! It's me, thread #%ld!\n", thread_id);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[THREAD_NUM];
    long thread_ids[THREAD_NUM];
    int rc;

    long i;
    for (i = 0; i < THREAD_NUM; ++i) {
        printf("main: creating thread %ld\n", i);
        thread_ids[i] = i;
        rc = pthread_create(&threads[i], NULL, print_hello, (void *)&thread_ids[i]);
        if (rc) {
            printf("error: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}


