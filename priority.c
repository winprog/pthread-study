#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>

const char *policy_name(int policy)
{
    return policy == SCHED_FIFO ? "FIFO" : 
        (policy == SCHED_RR ? "RR" : (
        policy == SCHED_OTHER ? "OTHER" : "UNKNOWN"));
 }

void *run(void *p)
{
    int policy;
    struct sched_param param;

    pthread_getschedparam(pthread_self(), &policy, &param);
    printf("run at %s/%d\n", policy_name(policy), param.sched_priority);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid;
    pthread_attr_t attr;
    int policy;

    struct sched_param param;
    int status;
    int min, max;

    pthread_attr_init(&attr);
    pthread_attr_getschedpolicy(&attr, &policy);
    pthread_attr_getschedparam(&attr, &param);
    printf("default at %s/%d\n", policy_name(policy), param.sched_priority);

    status = pthread_attr_setschedpolicy(&attr, SCHED_RR);
    if (status == 0)
    {
        min = sched_get_priority_min(SCHED_RR);
        max = sched_get_priority_max(SCHED_RR);
        // param.sched_priority = (min + max) / 2;
        param.sched_priority = max - 2;
        printf("min = %d, max = %d\n", min, max);
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    }

    pthread_create(&tid, &attr, run, NULL);
    pthread_join(tid, NULL);
    printf("main exit.\n");
    return 0;
}
