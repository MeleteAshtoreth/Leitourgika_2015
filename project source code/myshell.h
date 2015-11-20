#ifndef __MYSHELL__
#define __MYSHELL__
/* Include necessary header files */
#include <sys/wait.h> 	/* Declaration for waiting - for use of the waitpid() */
#include <unistd.h>		/* Provides access to the POSIX operating system API - for use of functions like fork(), pipe, read, write, chdir etc. */
#include <stdlib.h> 	/* General purpose standard library for functions involving memory allocation, process control, conversions and others. 
						   Also, defines maxros like EXIT_SUCCESS and EXIT_FAILURE. */
#include <stdio.h>		/* Standard Input and Output Library - use of functions like getchar(). */
#include <string.h>		/* Heador of functions that manipulate strings and arrays.*/

#define INPUT_SIZE 1024 /* This is size of the input from the user. */

/**
   Bultin command: change directory.
   args: List of args.  args[0] is "cd".  args[1] is the directory.
   Always returns 1, to continue executing.
 */
int myCd(char **args)
{
	if (args[1] == NULL) 
	{
    		// When the command cd has no argument then 
			// change to directory /home/$USER
		char newDirecory[INPUT_SIZE];
		memset(newDirecory, '\0', sizeof(newDirecory));
		// Get USER enviromental parameter
		char *user = getenv("USER");
		strcat(newDirecory, "/home/");
		strcat(newDirecory, user);
		chdir (newDirecory);
 	} 
 	else 
 	{
    	if (chdir(args[1]) != 0) 
    	{
      		perror("Error: ");
   		}
  	}
  
  return 1;
}

/**
   	Exit command
	This command just terminates the shell.
 */
int myExit()
{
	return 0;
}


int executeCommand(char *programName_)
{
	char *token;
	char **tokenisedArgs = malloc(INPUT_SIZE * sizeof(char*));
	int index = 0;
  	pid_t pid;
  	int status; 

  	token = strtok(programName_, " ");

 	while (token != NULL) 
 	{
   	 	tokenisedArgs[index] = token;
    	index++;
   		 //printf("index: %d %s\n", index, args[index]);
		token = strtok(NULL, " ");
  	}
  	tokenisedArgs[index] = NULL;

	if (strcmp(tokenisedArgs[0], "cd") == 0)
	{
      	return myCd(tokenisedArgs);
    }
    else if (strcmp(tokenisedArgs[0], "exit") == 0)
    {
      	return myExit();
    }
    else
    {
      	pid = fork();
 		if (pid == 0) 
 		{
   	 	// Child process
    		if (execvp(tokenisedArgs[0], tokenisedArgs) == -1) 
    		{
      			perror("Error");
    		}
    		exit(EXIT_FAILURE);
  		} 
  		else if (pid < 0) 
  		{
    		// Error forking
    		perror("Error PID");
  		} 
  		else 
  		{
    		// Parent process
    		do 
    		{
      			waitpid(pid, &status, WUNTRACED);
    		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  		}

    }
 	
  return 1;
}

#endif