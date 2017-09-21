#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define NUM_THREADS 25

pthread_attr_t attr;
pthread_t threads[NUM_THREADS];
pthread_mutex_t  threads_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t wait_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rand_lock = PTHREAD_MUTEX_INITIALIZER;
sem_t   death_lock;
pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  wait_cv = PTHREAD_COND_INITIALIZER;

int answer;

void delay(int a, int b)
{
    sleep(b);
}

void count_tries(int i)
{
    static int count = 0;
    static int old_count = 0;
    static int max_count = 0;

    static pthread_mutex_t count_lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&count_lock);
    count += i;
    if (i == -1) {
        printf("Total attempt count: %d\n", max_count);
    }
    pthread_mutex_unlock(&count_lock);
}

void cleanup_count(void *arg)
{
    int *ip = (int *)arg;
    int i = *ip;
    pthread_t tid = pthread_self();
    count_tries(i);
    printf("thread $%d: exited (or cancelled) on it's %d try.\n", (int)tid, i);
}

void cleanup_lock(void *arg)
{
    pthread_t tid = pthread_self();
    printf("thread #%d: freeing and releasing.\n", (int)tid);
    free(arg);
    pthread_mutex_unlock(&rand_lock);
}

void *search(void *arg)
{

  char *p;
    unsigned int seed;

    int i = 0, j, err, guess, target = (int)arg;


    pthread_t tid = pthread_self();
    seed = (unsigned int) tid;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_cleanup_push(cleanup_count, (void *)&i);


    while (1) {
        ++i;
        pthread_mutex_lock(&rand_lock);
        p  = (char *) malloc(10);
        pthread_cleanup_push(cleanup_lock, (void *)p);
        guess = rand_r(&seed);
        delay(0, 10);
        pthread_testcancel();
        pthread_cleanup_pop(0);
        free(p);
        pthread_mutex_unlock(&rand_lock);

        delay(0, 10);
    }

    pthread_cleanup_pop(0);
  return NULL;
}




int main(int argc, char *argv[])
{
    return 0;
}
