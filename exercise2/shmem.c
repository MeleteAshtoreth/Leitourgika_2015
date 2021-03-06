#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include <string.h>
#include <errno.h>

#include "display.h"

#define NUM_PROC 10

void * printmsg1(int * x, int id)
{
	int i=0;
	for (i; i < NUM_PROC; i++)
	{
		struct sembuf sem_op;
		
		if (*x == 0)
		{
			sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);

			display("ab");	
			*x = 1;
			// sleep(1);

            sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);

		}

		
	}
	shmdt((void *) x);
}

void * printmsg2(int * x, int id)
{
	int i=0;


	for (i; i < NUM_PROC; i++)
	{
		// printf("message2\n");
		struct sembuf sem_op;



		if (*x == 1)
		{
 
		    sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);

			display("cd\n");	
			*x = 0;		
			// sleep(4);	

		    sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);			
		}

	}
	shmdt((void *) x);
}

int main()
{
	    union semun 
    {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } sem_val;

    int semid1, semid2;
    int nsops;
    int nsems = 1;
    int semflg = IPC_CREAT | 0666;
    int rc;

    int childpid;
    int i;

    // struct sembuf wit,signal;
    struct sembuf *sops = (struct sembuf *) malloc( 2*sizeof(struct sembuf));

    // Create semaphore set with nsems semaphores in it
    // and with access only to the owner.
    if ( (semid1 = semget(semid1, 1, IPC_CREAT | 0600)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    sem_val.val = 1;
    if ( (rc = semctl(semid1, 0, SETVAL, sem_val)) == -1 )
    {
        perror("semget: semget failedl");
        exit(1);
    }

    if ( (semid2 = semget(semid2, 1, IPC_CREAT | 0666)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    if ( (rc = semctl(semid2, 0, SETVAL, sem_val)) == -1 )
    {
        perror("semget: semget failedl");
        exit(1);
    }

	int shmid = shmget(IPC_PRIVATE, sizeof(int), 0666);
	int *shared_int = (int *) shmat(shmid, 0, 0);
	*shared_int = 0;

	if ( fork() == 0)
	{
		printmsg1(shared_int, semid1);
		return 0;
	}
	else
	{
		printmsg2(shared_int, semid2);
	}
	

	for (i=0; i<NUM_PROC; i++) 
	{
        int child_status;

        wait(&child_status);
    }

     semctl(semid1,0,IPC_RMID);
     semctl(semid2,0,IPC_RMID);
	shmctl(shmid, IPC_RMID, 0);
	return 0;
}