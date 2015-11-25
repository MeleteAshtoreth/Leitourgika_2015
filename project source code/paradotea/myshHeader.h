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

char* getHomePath()
{
  char* homePath = malloc(sizeof(char) * INPUT_SIZE);
  
  // Get USER enviromental parameter
  char *user = getenv("USER");
  strcat(homePath, "/home/");
  strcat(homePath, user);
  strcat(homePath, "/");
  return homePath;
}

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
		// change to the $HOME directory /home/$USER
    // printf("Go to $user path\n");
		// char newDirecory[INPUT_SIZE];
		// memset(newDirecory, '\0', sizeof(newDirecory));
		// // Get USER enviromental parameter
		// char *user = getenv("USER");
		// strcat(newDirecory, "/home/");
		// strcat(newDirecory, user);
		chdir (getHomePath());
 	}
 	else 
  {
      char *relativePath = malloc(sizeof(char) * INPUT_SIZE);
      strcpy(relativePath, args[1]);
      
      if ((relativePath[0] == '~') && (relativePath[1] == '/'))
      {
        // E.g when the path is ~/Desktop.
        int start_Index = 2;
        int index;
        char *realPath = malloc(sizeof(char) * INPUT_SIZE);

        for (index=0; index < INPUT_SIZE; index++)
        {
          // Copy substring after ~/ 
          realPath[index] = relativePath[start_Index + index];
        }

      char *newPath = getHomePath();
      strcat(newPath, realPath);
      if (chdir(newPath) != 0) 
      {
        perror("Error ");
      }
    }
    else
    {
      int status;
      // printf("Bultin command!!!!\n");
      status = chdir(args[1]);
      if (status != 0) 
      {
          perror("Error ");
      }
    }
  }
  
  return 1;
}

/**
   	Exit command
	This command just terminates the shell.
 */
int myExit(char **args)
{
	if (args[1] != NULL)
	{
		// printf("No arguments should be given.\n");
		return 1;
	}
	return 0;
}


int executeCommand(char *commandName_)
{
	char *token;
	char **tokenisedArgs = malloc(INPUT_SIZE * sizeof(char*));
	int index = 0;
  pid_t pid;
  int status; 

  token = strtok(commandName_, " ");
  // printf("Command:%s\n", commandName_);
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
      	return myExit(tokenisedArgs);
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