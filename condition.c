#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_NUM  3
#define TCOUNT  10
#define COUNT_LIMIT 12

int count = 0;
int thread_ids[3] = {0, 1, 2};

pthread_mutex_t mutex;
pthread_cond_t cond;

void *inc_count(void *t)
{
    int i;
    long tid = (long)t;
    for (i = 0; i < TCOUNT; ++i) {
        pthread_mutex_lock(&mutex);
        ++count;

        if (count == COUNT_LIMIT) {
            pthread_cond_signal(&cond);
            printf("thread %ld: cond reached, count = %d.\n", tid, count);
        }

        printf("thread %ld: unlock mutex, count = %d.\n", tid, count);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *watch_count(void *p)
{
    long tid = (long)p;
    printf("thread %ld: watch_count() starting.\n", tid);
    pthread_mutex_lock(&mutex);
    while (count < COUNT_LIMIT) {
        pthread_cond_wait(&cond, &mutex);
        printf("thread %ld: watch_count() received signal.\n", tid);
    }

    count += 125;
    printf("thread %ld: watch_count() count = %d.\n", tid, count);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int i, rc;
    long t1 = 1;
    long t2 = 2;
    long t3 = 3;

    pthread_t threads[3];
    pthread_attr_t attr;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&threads[0], &attr, watch_count, (void*)t1);
    pthread_create(&threads[1], &attr, inc_count, (void*)t2);
    pthread_create(&threads[2], &attr, inc_count, (void*)t3);

    for (i = 0; i < THREAD_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("main: wait on %d threads.\n", i);

    pthread_attr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    pthread_exit(NULL);

    return 0;
}
