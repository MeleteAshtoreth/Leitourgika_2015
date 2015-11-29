#include "myshHeader.h"

int main(int argc, char **argv)
{
	int status;

	do
	{
		/*    Start implementantion.
		      It should stop when status is 0 (failure).	
		*/

		/* Set prompt as the '$' character.	*/
		printf("$");

        int inputSize = INPUT_SIZE; /* Set the buffer size of input. */
        int index = 0; /* Set the index of the input buffer. Initialise it to 0.*/
        int character;
        char *programName = malloc(sizeof(char) * inputSize); /* Allocate an array named programName of inputSize. 
                                                                 This will contain the input from the user. 
                                                              */

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

        // Here the programName contains 
        // the command and its arguments that will be executed.
		if (programName[0] != '\0')
		{
            // Check for pipes.
			if (strstr(programName, "|") != NULL) 
			{
                // We have to tokenize the programName using the | character
                // in order to get the input commands to execute the pipe.
				int i;
				int count = 0;
				for (i = 0; programName[i] != '\0'; i++) {
      					if (programName[i] == '|')
        				 count++;
   				}


                // In this shell only two pipes can be executed.
				if (count < 2)
				{
					status = execPipe(programName);
				}
				else
				{
					status = 1;
				} 
					
			}
			else 
			{
				status = executeCommand(programName);  
			}
		}
		
        // Free allocated memory.
        free(programName);

	}while(status);

  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
