/*-------------- mysh1 ---------------------*/

/* Include necessary header files */
#include <sys/wait.h> 	/* Declaration for waiting - for use of the waitpid() */
#include <unistd.h>		/* Provides access to the POSIX operating system API - for use of functions like fork(), pipe, read, write etc. */
#include <stdlib.h> 	/* General purpose standard library for functions involving memory allocation, process control, conversions and others. 
						   Also, defines maxros like EXIT_SUCCESS and EXIT_FAILURE. */
#include <stdio.h>		/* Standard Input and Output Library - use of functions like getchar(). */
#include <string.h>		/* Heador of functions that manipulate strings and arrays.*/

#define INPUT_SIZE 1024 /* This is size of the input from the user. */
#define TOKEN_DELIM " \t\r\n\a"

int launchProgram(char *programName_)
{
	char *token;
	char **args = malloc(INPUT_SIZE * sizeof(char*));
	int index = 0;
  	pid_t pid;
  	int status;

  	// token = strtok(programName_, "-");
  	// while(token != NULL)
  	// {
  	// 	printf("token is not null\n");
  	// 	printf( " %s\n", token );
 		// token = strtok(NULL, "-");

  	// 	args[index] = token;
  	// 	index++;


  	// }
  	// args[index] = NULL;
  	// printf("%s\n", token);
 	

 	  		/*------------------*/
  		  token = strtok(programName_, TOKEN_DELIM);

 		 while (token != NULL) {
   			 	args[index] = token;
    			index++;
   				 printf("index: %d %s\n", index, args[index]);

    			token = strtok(NULL, TOKEN_DELIM);
  			}
  			args[index] = NULL;

 	pid = fork();
 	if (pid == 0) {
   	 // Child process
    	if (execvp(args[0], args) == -1) {
      	perror("lsh");
    	}
    	exit(EXIT_FAILURE);
  		} else if (pid < 0) {
    		// Error forking
    		perror("lsh");
  		} else 
  		{
    		// Parent process
    		do {
      			waitpid(pid, &status, WUNTRACED);
    			} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  		}

  return 1;
}


/*
	This is the main funtion.
	Arguments:
		argc is a count of the arguments supplied to the program
		argv  is an array of pointers to the strings which are those arguments
	
	Return:
		status of the program (EXIT_SUCCESS or EXIT_FAILURE)
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
				programName[index] = '\0';
				break;
				// status = 1;
				// return status;
			}
			else if (character == ' ')
			{
				printf("Please give a valid program name. \nNo arguments should be given.\n");
				status = 0;
				return status;
			}
			else
			{
				programName[index] = character;
			}

			/* Set index of the programName array to the next read position. */
			index += 1;
		}

			printf("Idx = %d is %s\n", index, programName);

		status = launchProgram(programName);



	}while(status);


  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}