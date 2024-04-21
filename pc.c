#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


int main(int argc, char *argv[]){
    if(argc < 4){
        printf("Usage: %s num_producers num_consumers num_events_per_producer max_outstanding\n", argv[0]);
    }

    return 0;

}


void producer(){

}

void consumer(){

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