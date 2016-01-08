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

#define SEM_KEY 1234
#define NUM_PROC 100

void * print_ab(int id)
{
    int i;
        for (i =0; i < NUM_PROC; i++)
        {
            struct sembuf sem_op;

            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);

            display("ab");
            usleep(500000);

            sem_op.sem_num = 0;
            sem_op.sem_op = 1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);      
        }
}

void * print_cd(int id)
{
    int i;
        for (i =0; i < NUM_PROC; i++)
        {
            struct sembuf sem_op;

            sem_op.sem_num = 0;
            sem_op.sem_op = -1;
            sem_op.sem_flg = 0;
            semop(id, &sem_op, 1);

            display("cd\n");
            usleep(500000);

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
    if ( (semid1 = semget(semid1, 1, semflg)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    if ( (semid2 = semget(semid2, 1, semflg)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    sem_val.val = 1;
    if ( (rc = semctl(semid2, 0, SETVAL, sem_val)) == -1 )
    {
        perror("semget: semget failedl");
        exit(1);
    }
    if ( (rc = semctl(semid1, 0, SETVAL, sem_val)) == -1 )
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
        print_ab(semid1);

        return 0;
    }
    else
    {
        print_cd(semid2);
    }

   
    // semctl(semid,0,GETVAL, argument);
    // sleep(1);

    /* wait for all children to finish running */
    for (i=0; i<NUM_PROC; i++) {
        int child_status;

        wait(&child_status);
    }

     semctl(semid1,0,IPC_RMID);
     semctl(semid2,0,IPC_RMID);
    //  printf("\nSemaphore removed from the System = %s\n",strerror(errno));
    return 0;
}
