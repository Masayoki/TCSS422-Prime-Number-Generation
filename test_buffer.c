#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "buffer.h"

void *producer(void *arg)
{
    buffer_t *b = (buffer_t *)arg;
    
    // Produce items and put them in the buffer
    for (int i = 0; i < 10; i++) {
        printf("Producer: Producing item %d\n", i);
        put(i, b);
    }
    
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    buffer_t *b = (buffer_t *)arg;
    
    // Consume items from the buffer
    for (int i = 0; i < 10; i++) {
        int item = get(b);
        printf("Consumer: Consuming item %d\n", item);
    }
    
    pthread_exit(NULL);
}

int main()
{
    buffer_t b;
    initbuff(5, &b, "TestBuffer");

    pthread_t producer_thread;
    pthread_t consumer_thread;
    
    // Create producer thread
    if (pthread_create(&producer_thread, NULL, producer, &b) != 0) {
        fprintf(stderr, "Error creating producer thread\n");
        exit(1);
    }
    
    // Create consumer thread
    if (pthread_create(&consumer_thread, NULL, consumer, &b) != 0) {
        fprintf(stderr, "Error creating consumer thread\n");
        exit(1);
    }
    
    // Wait for threads to finish
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    freebuff(5, &b, "TestBuffer");
    
    return 0;
}

