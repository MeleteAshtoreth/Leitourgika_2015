// File: mutex.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "display.h"

#define NUM_THREADS 10000

unsigned int shared_data = 0;

unsigned int rc;
pthread_mutex_t printf_mutex = PTHREAD_MUTEX_INITIALIZER;

// try uncommenting and commenting the mutext below
// and look at the output

void print(char* a) {
  // pthread_mutex_lock(&printf_mutex); // comment out
  display(a);
  //display("cd\n");
  // pthread_mutex_unlock(&printf_mutex); // comment out
}


// These two functions will run concurrently.
void* print_ab(void *ptr) 
{
  rc = pthread_mutex_lock(&printf_mutex);
  int i;
  do 
  {
    if(shared_data%2 == 0)
    {
      print("ab");
      shared_data++;
    }
    else
    {
      rc = pthread_mutex_unlock(&printf_mutex);//if number is even, do not print, release mutex
    }
  }while(shared_data < NUM_THREADS);
}

void* print_cd(void *ptr) 
{
    rc = pthread_mutex_lock(&printf_mutex);
  int i;
  do
  {
    if(shared_data%2 != 0)
    {
      printf("cd\n");
      shared_data++;
    }
    else
    {
      rc=pthread_mutex_unlock(&printf_mutex);//if number is odd, do not print, release mutex
    }
  }while(shared_data < NUM_THREADS);
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

  pthread_mutex_destroy(&printf_mutex);

     /* Last thing that main() should do */
    pthread_exit(NULL);
  exit(0); //never reached.
}