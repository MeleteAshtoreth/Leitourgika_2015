#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "myshell.h"

char* readFromUser()
{
	int status;

	do
	{
		/*	Start implementantion.
		It should stop when status is 0 (failure).	
		*/

		/* Set prompt as the '$' character.	*/
		printf("$");

		int inputSize = INPUT_SIZE; /* Set the buffer size of input. */
		int index = 0; /* Set the index of the input buffer. Initialise it to 0.*/
		char *programName = malloc(sizeof(char) * inputSize); /* Allocate an array named programName of inputSize. This will contain the input from the user. */
		int character;

		/* 	Error checking.
			If the programName array is NULL then the program should terminate
			showing the appropriate message to the user.
		*/
		if (programName == NULL)
		{
			fprintf(stderr, "programName allocation error!!\n");
			status = 0;
		}
		
		while(1)
		{
			character = getchar(); /* Read characters from the user input. */

			/* 	Check character. */
			if (character == EOF || character == '\n')
			{
				programName[index] = '\0';
				break;
			}
			else
			{
				programName[index] = character;
			}

			/* Set index of the programName array to the next read position. */
			index += 1;

			status = 1;
		}  

			const char delim[2] = "|";
			if (strstr(programName, delim) != NULL) {
			// contains
			char *token;
			char command1[50];
			char command2[50];
			
			/* get the first token */
			token = strtok(programName, delim);
			strcpy(command1, token);
			
			/* walk through other tokens */
			while( token != NULL ) 
			{
			strcpy(command2, token);
			token = strtok(NULL, delim);
			
			}
			printf( "%s\n", command1 );
			printf( "%s\n", command2 );
			} 

	}while(status);
}

// int separateCommands(char* programName)
// {

// 			return 0;
// }
/**
 * Executes the command "cat scores | grep Villanova".  In this quick-and-dirty
 * implementation the parent doesn't wait for the child to finish and
 * so the command prompt may reappear before the child terminates.
 *
 * @author Jim Glenn
 * @version 0.1 9/23/2004
 */

int main(int argc, char **argv)
{
	int status;
	do
	{
		/*	Start implementantion.
		It should stop when status is 0 (failure).	
		*/

		/* Set prompt as the '$' character.	*/
		printf("$");

		int inputSize = INPUT_SIZE; /* Set the buffer size of input. */
		int index = 0; /* Set the index of the input buffer. Initialise it to 0.*/
		char *programName = malloc(sizeof(char) * inputSize); /* Allocate an array named programName of inputSize. This will contain the input from the user. */
		int character;

		/* 	Error checking.
			If the programName array is NULL then the program should terminate
			showing the appropriate message to the user.
		*/
		if (programName == NULL)
		{
			fprintf(stderr, "programName allocation error!!\n");
			status = 0;
		}
		
		while(1)
		{
			character = getchar(); /* Read characters from the user input. */

			/* 	Check character. */
			if (character == EOF || character == '\n')
			{
				printf("character: %d\n", character);
				programName[index] = '\0';
				break;
			}
			else
			{
				programName[index] = character;
			}

			/* Set index of the programName array to the next read position. */
			index += 1;

			status = 1;
		}  

	const char delim[2] = "|";
	if (strstr(programName, delim) != NULL) 
	{
		// contains
		char *token;
		char command1[50];
		char command2[50];
			
		/* get the first token */
		token = strtok(programName, delim);
		strcpy(command1, token);
			
		/* walk through other tokens */
		while( token != NULL ) 
		{
			strcpy(command2, token);
			memmove(command2, command2+1, strlen(command2));
			token = strtok(NULL, delim);
		}
			
		printf( "%s\n", command1 );
		printf( "%s\n", command2 );
			
		int pipefd[2];
		int pid;
			
			// char *cat_args[] = {"cat", "examplePipe.c", NULL};
			// char *grep_args[] = {"grep", "pid", NULL};
			
			// make a pipe (fds go in pipefd[0] and pipefd[1])
			
		pipe(pipefd);
			
		pid = fork();
      
        if (pid == -1)
        {
        	perror("fork");
       		exit(2);
        }
        else if (pid == 0)
        {
        // child gets here and handles "grep Villanova"
        
        // replace standard input with input part of pipe
        	printf("Child\n");
        	dup2(pipefd[0], 0);
        
        // close unused hald of pipe
        	executeCommand(command2);
        	close(pipefd[1]);
        
        // execute grep
        
        // execvp("grep", grep_args);	
        }
        else
        {
        // parent gets here and handles "cat scores"
        
        // replace standard output with output part of pipe
        	printf("Parent\n");
        	dup2(pipefd[1], 1);
        
        // close unused unput half of pipe
        	executeCommand(command1);
        	close(pipefd[0]);
        
        // execute cat
        
        // execvp("cat", cat_args);
        	
        }
    }
    else 
    {
    	status = executeCommand(programName);
    } 

	}while(status);

 


  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}