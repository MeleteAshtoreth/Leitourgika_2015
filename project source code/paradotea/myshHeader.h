#ifndef __MYSHELL__
#define __MYSHELL__
/* 
	This is the header file that includes necessary header files
	and defines common functions for the shells.
 */

#include <sys/wait.h> /* Declaration for waiting - 
                         for use of the waitpid() 
                      */
#include <unistd.h>		/*Standard symbolic constants and types 
                        for the use of functions like fork(), execvp(), pipe(), read(), write(), chdir() etc. 
                      */
#include <stdlib.h> 	/*Standard library definitions 
                        for functions involving memory allocation , process control, conversions and others. 
						            Also, defines macros like EXIT_SUCCESS, EXIT_FAILURE, NULL. 
                      */
#include <stdio.h>		/* Standard buffered input/output that declares 
                         macros such as EOF and functions like getchar(), fflush(). 
                      */
#include <string.h>		/* Header of functions that manipulate strings and arrays - for the use of
                         strtok() etc.
                      */

#define INPUT_SIZE 1024 // Define the maximum size of the input buffer.

/*
  This function returns a string
  containing the absolute home path 
  of the current user.
*/
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

/*
   Bultin command: change directory.
   This function implements the cd command.
   Input: A list of words (a sentence) containing the cd command
          and the directory.
          (args[0] is the cd command, args[1] is the directory)
   Return: 1, to continue executing.
 */
int myCd(char **args)
{
	if (args[1] == NULL) 
	{
    // When the command cd has no argument then 
		// change to the $HOME directory /home/$USER
		chdir (getHomePath());
 	}
 	else 
  {
    // Copy directory arguments to a new char.
    char *relativePath = malloc(sizeof(char) * INPUT_SIZE);
    strcpy(relativePath, args[1]);
      
    // When the user gives as an input directory 
    // with the following format, e.g ~/Desktop
    if ((relativePath[0] == '~') && (relativePath[1] == '/'))
    {
      int start_Index = 2;
      int index;
      char *realPath = malloc(sizeof(char) * INPUT_SIZE);

      for (index=0; index < INPUT_SIZE; index++)
      {
        // Copy substring after ~/ 
        realPath[index] = relativePath[start_Index + index];
      }

      // Change directory
      char *newPath = getHomePath();
      strcat(newPath, realPath);
      if (chdir(newPath) != 0) 
      {
        perror("Error ");
      }

      // Free memory from malloc.
      free(relativePath);
      free(realPath);
    }
    else
    {
      // When input is an absolute path.
      // Change directory
      if (chdir(args[1]) != 0) 
      {
          perror("Error ");
      }
    }
  }
  
  return 1;
}

/*
  Exit command
	This command just terminates the shell.
  Returns: 0, to terminate the shell
           1, when the user gives an argument to the exit command.
              An appropriate message should be printed.
 */
int myExit(char **args)
{
	if (args[1] != NULL)
	{
    perror("Error ");
		return 1;
	}

	return 0;
}

/*
   This is the main function of every shell that executes every command.
   In case of wrong input it should print an appropriate error message.
   Input: Given input from the user.
   Return: 1, to continue executing.
 */
int executeCommand(char *commandName_)
{
  int index = 0;
  int status;
  pid_t pid;
  
	char *token;
	char **tokenisedArgs = malloc(INPUT_SIZE * sizeof(char*));


  // Tokenize input using as a delimeter the white space character
  // to separate the command from its arguments.
  token = strtok(commandName_, " ");
 	while (token != NULL) 
 	{
   	tokenisedArgs[index] = token;
    index++;
		token = strtok(NULL, " ");
  }
  
  tokenisedArgs[index] = NULL;

  // Execute the command.
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
    // Create a child process by duplicating the parent process
    pid = fork();
 		if (pid == 0) 
 		{
   	 	// Child process
    	if (execvp(tokenisedArgs[0], tokenisedArgs) == -1) 
    	{
      			perror("Error");
            exit(EXIT_FAILURE);
    	}
    	
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
       // The waitpid() system call suspends execution of the calling process 
      // until a child specified by pid argument has changed state.
     			waitpid(pid, &status, WUNTRACED);
    	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
  	}

  }
 	
  return 1;
}


char ** tokenize(char *input)
{
    // Tokenize command
    char *token;
    char **tokenisedArgs = malloc(INPUT_SIZE * sizeof(char*));
    int index = 0;
                    
    token = strtok(input, " ");
    while (token != NULL) 
    {
        tokenisedArgs[index] = token;
        index++;
        token = strtok(NULL, " ");
    }
    tokenisedArgs[index] = NULL;
    
    // Free allocated memory.
    free(tokenisedArgs);

    return tokenisedArgs;                   
}

 /*
    This function performs an extra separation of the input to two different commands for the pipe
    and creates a child process to execute the pipe.
 */
void runPipeCmd(char *input1, char *input2) 
{
    int pipe_fd[2];
    int read_fd;
    int write_fd;
    int status;
    int pid;

    // Make a pipe (fds go in pipefd[0] and pipefd[1])
    if (pipe(pipe_fd)) 
    {
     perror("pipe");
     exit(127);
    }
    read_fd = pipe_fd[0];
    write_fd = pipe_fd[1];

    /*  At this point we are at the child process, 
        but since we have two commands to execute
        we must fork again.
     */
    switch ((pid = fork())) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        /* child process*/
        // Close the other part of the file descriptor of the pipe.
        close(read_fd); 
        // Duplicate the file descriptor
        dup2(write_fd, 1); 

        close(write_fd);

        char **cmd1 = malloc(INPUT_SIZE * sizeof(char*));
        cmd1 = tokenize(input1);

        // Execute command
        if (execvp(cmd1[0], cmd1) == -1)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        free(cmd1);

    default:
        /* parent process*/
        // Close stdin file descriptor of the pipe.
        close(write_fd);  
        // Duplicate the file descriptor
        dup2(read_fd, 0);  
        
        close(read_fd);  

        char **cmd2 = malloc(INPUT_SIZE * sizeof(char*));
        cmd2 = tokenize(input2); 

        // Execute command
        if (execvp(cmd2[0], cmd2) == -1)
        {
            perror("Error");
            exit(EXIT_FAILURE);
        }

        free(cmd2);

    }
 }
 
 /*
    This function separates the input buffer to two different commands for the pipe
    and creates a child process to execute the pipe.
    Input: the input from the user.
    Return: 1, to continue executing.
 */
int execPipe(char *inputCommands)
{
    int pid;
    int status;
    char *token;
    char firstCmd[INPUT_SIZE];
    char secondCmd[INPUT_SIZE];    

    /* 
        FLush the stdout buffer.
        Otherwise the string may not be sent through the pipe immediately.
    */
    fflush(stdout);  


    // Tokenize inputCommands to get the two commands of the pipe.
    token = strtok(inputCommands, "|");
    strcpy(firstCmd, token);
                    
    /* Walk through other tokens */
    while( token != NULL ) 
    {
        strcpy(secondCmd, token);
        memmove(secondCmd, secondCmd+1, strlen(secondCmd));
        token = strtok(NULL, "|");
    }

    switch ((pid = fork())) {
    case -1:
        perror("fork");
        break;
    case 0:
        /* child process*/
        runPipeCmd(firstCmd, secondCmd);
        break;
    default:
        // parent process

        do 
        {
            // The waitpid() system call suspends execution of the calling process 
            // until a child specified by pid argument has changed state.
                waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}
#endif
