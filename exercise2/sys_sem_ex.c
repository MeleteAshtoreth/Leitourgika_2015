#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "display.h"

int main()
{
	key_t		key;			/* key to pass to semget() */
 	int		semflg;			/* semflg to pass to semget() */
 	int		nsems;			/* nsems to pass to semget() */
 	int		semid;			/* return value from semget() */
		
 	key =  1492;
 	nsems = 1;
 	semflg = 0666 | IPC_CREAT;

 	if ( ( semid = semget ( key, nsems, semflg ) ) == –1 ) 
	{
 		perror("semget: semget failed");
 		exit(1);
 	} 
	else
	{
		exit(0);
	}
	
	if ( semctl(id, 0, SETVAL, argument) < 0)
	{
		fprintf( stderr, "Cannot set sem value.\n");
	}
	else
	{
		fprintf( stderr, "Semaphore %d initialised.\n", KEY);
	}
	
	return 0;
}
