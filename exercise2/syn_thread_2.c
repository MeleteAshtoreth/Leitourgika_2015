// File: mutex.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "display.h"

#define NUM_THREADS 10

unsigned int shared_data = 0;

unsigned int rc;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void print(char* a) 
{
  display(a);
}

void* print_ab(void *ptr) 
{
  
  int i;
  do 
  {
    if(shared_data%2 == 0)
    {
      rc = pthread_mutex_lock(&lock);
      print("ab");
      shared_data++;
      // printf("\nShared dataAB: %d", shared_data);
      pthread_cond_signal(&cond);
    }
    else
    {
      pthread_cond_wait(&cond, &lock);
    }
    rc = pthread_mutex_unlock(&lock);//if number is even, do not print, release mutex
  }while(shared_data < 2*NUM_THREADS);
}

void* print_cd(void *ptr) 
{
    
  int i;
  do
  {
    rc = pthread_mutex_lock(&lock);
    if(shared_data%2 != 0)
    {
      print("cd\n");
      shared_data++;
      // printf("\nShared dataCD: %d", shared_data);
      pthread_cond_signal(&cond);
    }
    else
    {
      pthread_cond_wait(&cond, &lock);
    }
    rc=pthread_mutex_unlock(&lock);//if number is odd, do not print, release mutex
  }while(shared_data < 2*NUM_THREADS);
}

int main() {
  int rc1, rc2;
  pthread_t t1, t2;


    if ( (rc1 = pthread_create(&t1, NULL, print_ab, NULL)) == -1)
    {
      perror("create thread 1");
      exit(0);
    }

    if ( (rc2 = pthread_create(&t2, NULL, print_cd, NULL)) == -1)
    {
      perror("create thread 2");
      exit(0);
    }

  //sleep(3);

  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  pthread_mutex_destroy(&lock);

     /* Last thing that main() should do */
    pthread_exit(NULL);
  exit(0); //never reached.
}