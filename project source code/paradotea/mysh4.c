#include "myshHeader.h"

char ** tokenize(char *input)
{
    // Tokenize command
    char *token;
    char **tokenisedArgs = malloc(INPUT_SIZE * sizeof(char*));
    int index = 0;
                    
    token = strtok(input, " ");
    // printf("Command:%s\n", command2);
    while (token != NULL) 
    {
        tokenisedArgs[index] = token;
        index++;
        //printf("index: %d %s\n", index, args[index]);
        token = strtok(NULL, " ");
    }
    tokenisedArgs[index] = NULL;
    return tokenisedArgs;                   
}

void docommand(char *inp1, char *inp2)  /* does not return, under any circumstances */
{
    int pipefd[2];

    /* get a pipe (buffer and fd pair) from the OS */
    if (pipe(pipefd)) {
	perror("pipe");
	exit(127);
    }

    /* We are the child process, but since we have TWO commands to exec we
     * need to have two disposable processes, so fork again */
    switch (fork()) {
    case -1:
        perror("fork");
        exit(127);
    case 0:
        /* child */
        /* do redirections and close the wrong side of the pipe */
        close(pipefd[0]);  /* the other side of the pipe */
        dup2(pipefd[1], 1);  /* automatically closes previous fd 1 */
        close(pipefd[1]);  /* cleanup */

        char **cmd1 = malloc(INPUT_SIZE * sizeof(char*));
        cmd1 = tokenize(inp1);

        execvp(cmd1[0], cmd1);

        perror("Error");
        exit(126);
    default:
        /* parent */

        /* do redirections and close the wrong side of the pipe */
        close(pipefd[1]);  /* the other side of the pipe */
        dup2(pipefd[0], 0);  /* automatically closes previous fd 0 */
        close(pipefd[0]);  /* cleanup */

        char **cmd2 = malloc(INPUT_SIZE * sizeof(char*));
        cmd2 = tokenize(inp2); 

        execvp(cmd2[0], cmd2);
        perror("Error");
        exit(125);
    }
 }
 
int execPipe(char *command)
{
    int pid;
    int status;
	fflush(stdout); 


    char *token;
    char command1[50];
    char command2[50];
    
    /* get the first token */
    token = strtok(command, "|");
    strcpy(command1, token);
                    
    /* walk through other tokens */
    while( token != NULL ) 
    {
        strcpy(command2, token);
        memmove(command2, command2+1, strlen(command2));
        token = strtok(NULL, "|");
    }

    switch ((pid = fork())) {
    case -1:
        perror("fork");
        break;
    case 0:
        /* child */
        docommand(command1, command2);
        break;  /* not reached */
    default:
        /* parent; fork() return value is child pid */

        pid = wait(&status);

    }

    return 1;
}

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
			// fprintf(stderr, "programName allocation error!!\n");
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
		}

		// Here the programName variable contains the name of the program 
		// that the user wants to execute.
		if (programName[0] != '\0')
		{
			if (strstr(programName, "|") != NULL) 
			{
				status = execPipe(programName);
			}
			else 
			{
				status = executeCommand(programName);  
			}
		}
		//printf("Status:%d\n", status);
	}while(status);

  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
