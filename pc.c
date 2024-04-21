#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "eventbuf.h"
struct eventbuf *eb;
int num_events_per_producer;
sem_t *producer_sem;
sem_t *consumer_sem;
sem_t *mutex;

void* producer(void *arg){
    int tid = *(int *)arg;
    for (int i = 0; i < num_events_per_producer; i++){
        sem_wait(producer_sem);
        sem_wait(mutex);
        printf("P%d: added event %d\n", tid, tid*100+i);
        eventbuf_add(eb, i);
        sem_post(mutex);
        sem_post(consumer_sem);
    }
    printf("P%d: exiting\n", tid);
    pthread_exit(NULL);
}

void* consumer(void *arg){
    int tid = *(int *)arg;
    while(1){
        sem_wait(consumer_sem);
        sem_wait(mutex);
        if (eventbuf_empty(eb)){
            sem_post(mutex);
            break;
        }
        int event = eventbuf_get(eb);
        printf("C%d: got event %d\n", tid, event);
        sem_post(mutex);
        sem_post(producer_sem);
    
    }
    printf("C%d: exiting\n", tid);
    pthread_exit(NULL);
}

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

int main(int argc, char *argv[]){
    if(argc < 4){
        printf("Usage: %s num_producers num_consumers num_events_per_producer max_outstanding_events\n", argv[0]);
        exit(1);
    }
    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    num_events_per_producer = atoi(argv[3]);
    int max_outstanding_events = atoi(argv[4]);

    eb = eventbuf_create();
    producer_sem = sem_open_temp("producer_sem", max_outstanding_events);
    consumer_sem = sem_open_temp("consumer_sem", 0);
    mutex = sem_open_temp("mutex", 1);

    pthread_t *producer_threads = calloc(num_producers, sizeof(pthread_t));
    pthread_t *consumer_threads = calloc(num_consumers, sizeof(pthread_t));
    int *producer_tids = calloc(num_producers, sizeof(int));
    int *consumer_tids = calloc(num_consumers, sizeof(int));

    for (int i = 0; i < num_producers; i++){
        producer_tids[i] = i;
        pthread_create(producer_threads+i, NULL, &producer, producer_tids+i);
    }

    for (int i = 0; i < num_consumers; i++){
        consumer_tids[i] = i;
        pthread_create(consumer_threads+i, NULL, &consumer, consumer_tids+i);
    }

    for (int i = 0; i < num_producers; i++){
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++){
        sem_post(consumer_sem);
    }

    for (int i = 0; i < num_consumers; i++){
        pthread_join(consumer_threads[i], NULL);
    }

    eventbuf_free(eb);
    return 0;

}


