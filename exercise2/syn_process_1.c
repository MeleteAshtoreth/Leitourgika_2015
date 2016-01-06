#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include "display.h"

#define NUM_PROC 10
#define SEM_NUM 2

int main()
{
	pid_t child;
	int i, rc;
	
	// Semaphore parameters
	int sem_id;
	struct sembuf sem_op;
	union semun
	{
		int val;
		struct semid_ds *buf;
		unsigned short int *array;
		struct seminfo *_buf;
	} sem_arg;
	
	// Allocate semaphore
	sem_id = semget(IPC_PRIVATE,  SEM_NUM, 0600 | IPC_CREAT );
	if ( sem_id == -1 )
	{
		perror("Semaphore initialization.\n");
		exit(1);
	}
	
	
	if (fork())
	{
		//printf("Child\n");
	
				// Initialize semaphore
			unsigned short values[1];
			values[0] = 1;
			sem_arg.array = values;
			//printf("Sem %d\n", i);
			sem_arg.val = 1;
			rc = semctl(sem_id,  NUM_PROC, SETVAL, sem_arg);
			if (rc == -1)
			{
				perror("Main: semctl");
				exit(1);
			}
			
			
		for (i=0; i<  NUM_PROC; i++)
 		{
		
			sem_op.sem_num = i;
			sem_op.sem_op = -1;
			sem_op.sem_flg = 0;
			semop(sem_id, &sem_op, 1);
	
			display("Hello world\n");
			wait(NULL);
			
			// release semaphore
			sem_op.sem_num = i;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;
			semop(sem_id, &sem_op, 1);
			}
			
						  // Deallocate semaphore
			rc = semctl(sem_id, NUM_PROC, IPC_RMID, sem_arg);
			//printf("Child Deallocate sem rc: %d\n", rc);
  }
  else
  {
	  //printf("Parent\n");
	
					// Initialize semaphore
		/*	unsigned short values[1];
			values[0] = 1;
			sem_arg.array = values;
			//printf("Sem %d\n", i);
			sem_arg.val = 1;
			rc = semctl(sem_id,  1, SETVAL, sem_arg);
			if (rc == -1)
			{
				perror("Main: semctl");
				exit(1);
			}
			*/
	for (i = 0; i < NUM_PROC; i++)
	{	
			/*sem_op.sem_num = 1;
			sem_op.sem_op = -1;
			sem_op.sem_flg = 0;
			semop(i, &sem_op, 1);
	*/
			display("Kalimera kosme\n");
			
		/*	// release semaphore
			sem_op.sem_num = 0;
			sem_op.sem_op = 1;
			sem_op.sem_flg = 0;
			semop(i, &sem_op, 1);
			*/

		}
					  // Deallocate semaphore
			//rc = semctl(1, 1, IPC_RMID, sem_arg);
			//printf("Parent Deallocate sem rc: %d\n", rc);
			
	}
  
  return 0;
}
