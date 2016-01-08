#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>

#include <string.h>
#include <errno.h>

#include "display.h"

#define NUM_PROC 1000

void * printmsg1(int id)
{
	int i=0;
	for (i; i < NUM_PROC; i++)
	{
		struct sembuf sem_op;

            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);
		
			display("Hello world\n");	
            

            sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);


	}
}

void * printmsg2(int id)
{
	int i=0;
	for (i; i < NUM_PROC; i++)
	{
		struct sembuf sem_op;

            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);
		
			display("Kalimera kosme\n");	
            

            sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);
	}
}

int main()
{
	union semun 
    {
        int val;
        struct semid_ds *buf;
        ushort *array;
    } sem_val;

    int semid;
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
    if ( (semid = semget(semid, 1, IPC_CREAT | 0600)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    sem_val.val = 1;
    if ( (rc = semctl(semid, 0, SETVAL, sem_val)) == -1 )
    {
        perror("semget: semget failedl");
        exit(1);
    }


	if ( fork() == 0)
	{
		printmsg1(semid);
	}
	else
	{
		printmsg2(semid);
	}

	for (i=0; i<NUM_PROC; i++) 
	{
        int child_status;

        wait(&child_status);
    }

	semctl(semid,0,IPC_RMID);
	return 0;
}