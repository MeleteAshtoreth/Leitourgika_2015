#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "display.h"

#define NUM_THREADS	10

pthread_mutex_t  printf_mutex = PTHREAD_MUTEX_INITIALIZER;

void *print_function(void *arg)
{
	char *p = arg;
	
	pthread_mutex_lock(&printf_mutex);
	display(p);
	pthread_mutex_unlock(&printf_mutex);
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   int i, j;
	
	char *msg1 = "Kalimera kosme\n";
	char *msg2 = "Hello world\n";

	//pthread_mutex_init(&printf_mutex, NULL);

	for(i=0; i<NUM_THREADS; i++)
 	{
     		rc = pthread_create(&threads[i], NULL, print_function,  (void *)msg1);
  		rc = pthread_create(&threads[i], NULL, print_function,  (void *)msg2);

     		if (rc)
		{
       			printf("ERROR; return code from pthread_create() is %d\n", rc);
       			exit(-1);
      		}
  	}


   	for(j=0; j < NUM_THREADS; j++)
   	{
     		pthread_join( threads[j], NULL); 
  	}

	pthread_mutex_destroy(printf_mutex);

  	 /* Last thing that main() should do */
   	pthread_exit(NULL);
}