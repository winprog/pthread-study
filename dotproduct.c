#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double *a;
    double *b;
    double sum;
    int len;
} DOTDATA;

#define THREAD_NUM 100 
#define VECLEN  10000

DOTDATA dotstr;
pthread_t dot_threads[THREAD_NUM];
pthread_mutex_t mutex;

void *dotprod(void *arg)
{
    int i, start, end, len;
    long offset;
    double sum, *x, *y;
    
    offset = (long)arg;
    len = dotstr.len;
    start = offset * len;
    end = start + len;
    x = dotstr.a;
    y = dotstr.b;

    sum = 0.0;
    for (i = start; i < end; ++i) {
        sum += x[i] * y[i];
    }
    printf("thread %ld: sum = %f\n", offset, sum);

    pthread_mutex_lock(&mutex);
    dotstr.sum += sum;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    long i;
    double *a, *b;
    void *status;
    pthread_attr_t attr;

    a = (double *)malloc(THREAD_NUM * VECLEN * sizeof(double));
    b = (double *)malloc(THREAD_NUM * VECLEN * sizeof(double));

    for (i = 0; i < VECLEN * THREAD_NUM; ++i) {
        a[i] = b[i] = 1.0;
    }

    dotstr.len = VECLEN;
    dotstr.a = a;
    dotstr.b = b;
    dotstr.sum = 0.0;
    
    pthread_mutex_init(&mutex, NULL);

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    for (i = 0; i < THREAD_NUM; ++i) {
        pthread_create(&dot_threads[i], &attr, dotprod, (void *)i);
    }
    pthread_attr_destroy(&attr);

    for (i = 0; i < THREAD_NUM; ++i) {
        pthread_join(dot_threads[i], &status);
    }

    printf("sum = %f \n", dotstr.sum);
    free(a);
    free(b);
    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

    return 0;
}
