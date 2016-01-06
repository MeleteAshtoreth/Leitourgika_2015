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

#define SEM_KEY 1234
#define NUM_PROC 10

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


    childpid = fork();

    if (childpid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (childpid == 0)
    {     
        int i;
        for (i =0; i < NUM_PROC; i++)
        {
            struct sembuf sem_op;

            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(semid, &sem_op, 1);

            display("Hello world\n");

            sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(semid, &sem_op, 1);            
        }

        return 0;
    }
    else
    {
        int j;
        for (j = 0; j < NUM_PROC; j++)
        {
        // Parent
        struct sembuf sem_op;

        sem_op.sem_num = 0;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);

        display("Kalimera kosme\n");


        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);            
        }


    }

   
    // semctl(semid,0,GETVAL, argument);
    // sleep(1);

    /* wait for all children to finish running */
    for (i=0; i<NUM_PROC; i++) {
        int child_status;

        wait(&child_status);
    }

     semctl(semid,0,IPC_RMID);
    //  printf("\nSemaphore removed from the System = %s\n",strerror(errno));
    return 0;
}
