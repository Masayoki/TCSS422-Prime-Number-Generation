/*
 *  Bounded buffer skeleton for Sieve of Erathosthenes primes program
 *  Based on Operating Systems: Three Easy Pieces by R. Arpaci-Dusseau and A. Arpaci-Dusseau
 * 
 *  This example uses locks and a condition to synchronize a child thread and
 *  the parent.  The child generates matricies and the parent computes the
 *  average when the matrix is ready for processing.
 *
 *  Wes J. Lloyd
 *  University of Washington, Tacoma
 *  TCSS 422 - Operating Systems
 *  Spring 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sched.h>
#include <string.h>
#include "buffer.h"

void initbuff(int nslots, buffer_t *buff, char * n)
{
    buff->buffer = (int *)malloc(nslots * sizeof(int));
    buff->maxSlots = nslots;
    buff->use_ptr = 0;
    buff->fill_ptr = 0;
    buff->count = 0;
    buff->name = strdup(n);

    pthread_mutex_init(&(buff->mutex), NULL);	//Initialize the mutex lock for the buffer
    pthread_cond_init(&(buff->cond), NULL);	//Initialize the condition variable for the buffer
}

void freebuff(int nslots, buffer_t *buff, char * n)
{
    free(buff->buffer);	//Free the memory allocated for the buffer
    pthread_mutex_destroy(&(buff->mutex));	//Destroy the mutex lock for the buffer
    pthread_cond_destroy(&(buff->cond));	//Destroy the condition variable for the buffer
    free(buff->name);	//Free the memory allocated for the buffer's name
}

void put(int value, buffer_t *buff)
{
    pthread_mutex_lock(&(buff->mutex));	//Acquire the mutex lock for the buffer
    while (buff->count == buff->maxSlots)
    {
        pthread_cond_wait(&(buff->cond), &(buff->mutex));	//Wait if the buffer is full
    }

    buff->buffer[buff->fill_ptr] = value;	//Put the value into the buffer
    buff->fill_ptr = (buff->fill_ptr + 1) % buff->maxSlots;	//Update the fill pointer
    buff->count++;	//Increment the count of elements in the buffer

    pthread_cond_signal(&(buff->cond));	//Signal to waiting threads that the buffer has data
    pthread_mutex_unlock(&(buff->mutex));	//Release the mutex lock
}

int get(buffer_t *buff)
{
    pthread_mutex_lock(&(buff->mutex));	//Acquire the mutex lock for the buffer
    while (buff->count == 0)
    {
        pthread_cond_wait(&(buff->cond), &(buff->mutex));	//Wait if the buffer is empty
    }

    int value = buff->buffer[buff->use_ptr];	//Get the value from the buffer
    buff->use_ptr = (buff->use_ptr + 1) % buff->maxSlots;	//Update the use pointer
    buff->count--;	//Decrement the count of elements in the buffer

    pthread_cond_signal(&(buff->cond));	//Signal to waiting threads that the buffer has space
    pthread_mutex_unlock(&(buff->mutex));	//Release the mutex lock
    
    return value;	//Return the retrieved value
}

