/*
 *  Bounded buffer header file
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

#define OUTPUT 0

#define BOUNDED_BUFFER_SIZE 10

typedef struct __buffer_t {
  int * buffer;           // ptr to content of bounded buffer
  int maxSlots;           // size of bounded buffer
  int use_ptr;            // next item to consume
  int fill_ptr;           // next location to fill
  int count;              // number of elements in bounded buffer
  char * name;            // unique name of this bounded buffer (for debugging purposes)
  pthread_mutex_t mutex;  // this buffer's mutex lock variable
  pthread_cond_t cond;    // this buffer's condition variable
} buffer_t;


void initbuff(int nslots, buffer_t *buff, char * n);

void freebuff(int nslots, buffer_t *buff, char * n);

void put(int value, buffer_t *buff);

int get(buffer_t *buff);

