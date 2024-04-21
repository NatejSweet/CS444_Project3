# Project 3

## Building

Command line:

* `make` to build. An executable called `pc` will be produced.
* `make clean` to clean up all build products.

## Files

* `pc.c`: The main code to launch the program
* `eventbuf.c`: The main code that handles the event buffer

## Data

There are three semaphores in the code, `consumer`, `producer` and `mutex`, mutext is used to lock the event buffer, while consumer and producer are used to signal to the threads they are named for the wake up. 

There are also arrays of thread ids and threads, created using calloc to prevent segmentation faults.

## Functions

* `main()`
    * `eventbuf_create()` - creates the event buffer
    * `sem_open_temp()` - creates named semaphores
    * `pthread_create()` - creates a new thread, launching it on the passed function
    * `consumer()` - the consumer thread function, will loop, removing items from the event buffer 1 by 1
    * `producer()` - the producer thread function, iterates up the the number of events per producer, adding items to the buffer 1 by 1 
    * `pthread_join()` - causes the thread that calls it to wait for the thread passed in to complete
    * `eventbuf_free()` - frees the event buffer

## Notes

Usage: ./pc  num_producers num_consumers num_events_per_producer max_outstanding_events