#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <assert.h>

/***************************************************
*
* Homework 7, Problem 2
* CPSC 435, Section II
* Chris A. Smith
* 
* family.cpp
*
* Creates four processes: parent, child1, child 2,
* and grandchild.  Each child process sleeps a random amount
* of time and then prints its name and pid to stdout.  
* 
***************************************************/

using namespace std;

static int semaphore;
static sembuf acquire = {0,-1, SEM_UNDO};
static sembuf release = {0, 1, SEM_UNDO};


/***************************************************
*
* Function child1
*
* This function encapsulates the child1 process code.
* child1 forks, resulting in the original child1 process
* and a grandchild process.  The grandchild process sleeps
* for a random amount of time, then prints its name
* and pid and exits unconditionally.  The child1
* process waits for the grandchild to complete, then 
* sleeps a random time, prints its name and pid, and exits.
* 
***************************************************/
int child1(void)
{
	int pid;
	
	// Acquire semaphore
	assert(semctl(semaphore, 0, GETVAL, 0) == 1);
	if (semop(semaphore, &acquire, 1) != 0)
	{
		perror("child1");
		return 2;
	}
	
	// Fork grandchild
	pid = fork();
	
	// grandchild code
	if (pid == 0) 
	{
		usleep(rand()%128);
		printf("grandchild %d\n", getpid());
		return 0;
	}
	
	// child1 code
	else if (pid > 0)
	{
		wait(NULL);  // Wait for grandchild to complete
		usleep(rand()%128);
		printf("child1 %d\n", getpid());
		if (semop(semaphore, &release, 1) != 0)  // Release semaphore
		{
			perror("child1");
			return 3;
		}
		return 0;
	}
	
	// On fork error, print message and exit
	else
	{
		perror("family (child1)");
		semop(semaphore, &release, 1); // Release semaphore
		return 1;
	}
}


/***************************************************
*
* Function main
*
* main is the entry point for the family program.
* This function forks once to create the child1 process
* (which calls the function child1()).  The parent process
* then forks again to create the child2 process.
*
* The child2 process waits for child1 to complete,
* then sleeps a random time before printing its own 
* name and pid to stdout.
*
* The parent process waits for both child1 and child2 to
* complete, and then prints its own name and pid to stdout.
*
***************************************************/
int main(int argc, char *argv[])
{
	int pid;  // process id 
	key_t key;  // semaphore key
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort *array;
	} semargs;
	
	// Create semaphore
	key = ftok(".",'f');
	if ((semaphore = semget(key, 1, IPC_CREAT | 0600)) < 0)
	{
		perror("family");
		return 2;
	}
	
	semargs.val = 1;
	if (semctl(semaphore, 0, SETVAL, semargs) != 0)
	{
		perror("family");
		return 3;
	}
	assert(semctl(semaphore, 0, GETVAL, 0) == 1);

	
	// Fork child1
	pid = fork();
	
	// child1 code
	if (pid == 0) 
		return child1();
	
	// parent code
	else if (pid > 0)
	{
		pid = fork();  //Fork child2
		
		// child2 code
		if (pid == 0)
		{
			semop(semaphore, &acquire, 1);  // block until child1 completes
			usleep(rand()%128);
			printf("child2 %d\n", getpid());
		}
		// parent code
		else if (pid > 0)  
		{
			waitpid(pid, NULL, 0); // Wait for child2 to complete
			printf("parent %d\n", getpid());
			if (semctl(semaphore, 0, IPC_RMID, NULL) < 0)
			{
				perror("family");
				return 4;
			};
		}
		return 0;
	}	
	
	// On fork error, print message and exit
	else
	{
		perror("family");
		return 1;
	}
		
}
