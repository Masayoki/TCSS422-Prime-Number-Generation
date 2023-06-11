/*
 *  Prime starter code for Sieve of Erathosthenes primes algorithm 
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

void *worker(void *arg)
{
  int n;
  int myprime; 
  char name[10];
  pthread_t thread;
  buffer_t out;
  buffer_t * b = (buffer_t *) arg;

  // the first int seen will be prime, because it passed all previous filters
  myprime = get(b);
  sprintf(name,"%d",myprime);   // assign the name of the buffer to be the prime number associated with it
  if (myprime < 0) 
  {
    return (void *) NULL;       // -1 signals to exit - prime number generation has finished
  }
  else
  {
    printf("%d ", myprime);  // print prime number to console
//    printf("%s\n", b->name);  // alternatively print the bounded buffer's name

    // initialize new bounded buffer for next worker
    initbuff(BOUNDED_BUFFER_SIZE, &out, name);	

    // Setup next worker thread to process primes in the chain
    pthread_create(&thread, NULL, worker, &out);  

    n = get(b);
    while ( n >= 0)
    {
      if (n % myprime != 0) // Filter out multiples of myprime
      {
        put(n,&out);	      
      }
      n = get(b);  // output the negative value to end
    }
    put(n,&out);
  }
  pthread_join(thread, NULL);
  freebuff(BOUNDED_BUFFER_SIZE, &out, b->name);  // this is optional
  return (void *) NULL;
}

int main (int argc, char * argv[])
{
  //char prompt[100];
  //printf("PID=%d\nPress [ENTER] to continue\n",getpid());
  //scanf("%s",prompt);
  
  int prime_count = 50000;	// exceeding 50,000 is not recommended
  				// often will result in overrun of system resources for creating pthreads
  if (argc>1)
  {
    prime_count = atoi(argv[1]);
  }
  printf("main: start \n");
  printf("PRIMES:\n1 ");
  char name[10];
  sprintf(name,"%d",1);		// set the name of the first buffer to be 1.
  buffer_t b; 

  // initialize the first buffer
  initbuff(BOUNDED_BUFFER_SIZE, &b, name);

  // create the first worker thread
  pthread_t p1;
  pthread_create(&p1, NULL, worker, &b);  

  // put initial data into bounded buffer
  for (int i=2; i <= prime_count; i++)
  {
    put(i,&b);
  }
  put(-1,&b);

  pthread_join(p1, NULL);
  printf("\nmain: end \n");
  return 0;
}
