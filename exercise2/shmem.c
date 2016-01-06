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

int main()
{
	int shmid = shmget(IPC_PRIVATE, sizeof(int), 0600);
	int *shared_int = (int *) shmat(shmid, 0, 0);
	*shared_int = 0;

	if ( fork() == 0)
	{
		int i;
		for (i = 0; i < 5; i++)
		{
			*shared_int = 1;
			display("Hello world\n");
			*shared_int = 0;
		}
	
		
		shmdt((void *) shared_int);	

	}
	else
	{
		sleep(1);
		int i;
		for (i = 0; i < 5; i++)
		{
			if (*shared_int == 0)
			{
				display("Kalimera kosme\n");
				*shared_int = 0;
			}
		}
		
		shmdt((void *) shared_int);
		shmctl(shmid, IPC_RMID, 0);
	}

	return 0;
}