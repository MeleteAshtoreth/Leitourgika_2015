#include "myshell.h"

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
        /* exec ls */
        // execl("/bin/ls", "ls", (char *)NULL);
        execvp(cmd1[0], cmd1);
        /* return value from execl() can be ignored because if execl returns
         * at all, the return value must have been -1, meaning error; and the
         * reason for the error is stashed in errno */
        perror("/bin/ls");
        exit(126);
    default:
        /* parent */
        /*
         * It is important that the last command in the pipeline is execd
         * by the parent, because that is the process we want the shell to
         * wait on.  That is, the shell should not loop and print the next
         * prompt, etc, until the LAST process in the pipeline terminates.
         * Normally this will mean that the other ones have terminated as
         * well, because otherwise their sides of the pipes won't be closed
         * so the later-on processes will be waiting for more input still.
         */
        /* do redirections and close the wrong side of the pipe */
        close(pipefd[1]);  /* the other side of the pipe */
        dup2(pipefd[0], 0);  /* automatically closes previous fd 0 */
        close(pipefd[0]);  /* cleanup */

        char **cmd2 = malloc(INPUT_SIZE * sizeof(char*));
        cmd2 = tokenize(inp2); 
        /* exec tr */
        // execl("/usr/bin/tr", "tr", "e", "f", (char *)NULL);
        execvp(cmd2[0], cmd2);
        perror("/usr/bin/tr");
        exit(125);
    }

    /*
     * When the exec'd processes exit, all of their file descriptors are closed.
     * Thus the "ls" command's side of the pipe will be closed, and thus the
     * "tr" command will get eof on stdin.  But if we didn't have the
     * close(pipefd[1]) for 'tr' (in the default: case), the incoming side
     * of the pipe would NOT be closed (fully), the "tr" command would still
     * have it open, and so tr itself would not get eof!  Try it!
     */
 }
 
int execPipe(char *command)
{
    int pid;
    int status;
	fflush(stdout);  /* important, otherwise the stdout buffer would be
                      * present in both processes after the fork()!
                      * It could be printed twice...  Or never printed,
                      * because of the exec overwriting this whole process.
                      * It depends on how it's being buffered.  When doing
                      * a fork or exec, we are careful to empty our stdio
                      * buffers first.  */


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
        /* These two pids output below will be the same: the process we
         * forked will be the one which satisfies the wait().  This mightn't
         * be the case in a more complex situation, e.g. a shell which has
         * started several "background" processes. */
        // printf("fork() returns child pid of %d\n", pid);
        pid = wait(&status);
        // printf("wait() returns child pid of %d\n", pid);
        // printf("Child exit status was %d\n", status >> 8);
                /* status is a two-byte value; the upper byte is the exit
                 * status, i.e. return value from main() or the value passed
                 * to exit(). */
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
		}

		// Here the programName variable contains the name of the program 
		// that the user wants to execute.
		// char delim[2] = "|";
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
		printf("Status:%d\n", status);
	}while(status);

  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}