/*-------------- mysh1 ---------------------*/
#include "myshHeader.h"


// Compile command: gcc -Wall mysh1.c -o mysh1


/*
	This is the main funtion.
	Input:
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
		int character;
		char *programName = malloc(sizeof(char) * inputSize); /* Allocate an array named programName of inputSize. 
																 This will contain the input from the user. 
															  */
		
		while(1)
		{
			character = getchar(); // Read characters from the user input. 

			/* 	Check character. */
			if (character == EOF || character == '\n')
			{
				programName[index] = '\0';
				break;
			}
			else if (character == ' ')
			{
				/* 	When the user writes a command with arguments
					then an appropriate message is printed.
					This also happens when he/she writes the command cd with arguments.
					This should not happen generally, but there is no explanation in the instructions
					of the exercise. 
				*/
				continue;
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
			status = executeCommand(programName);    

		// Free memory from malloc.
		free(programName);

	}while(status);



  // Perform any shutdown/cleanup.
  return EXIT_SUCCESS;
}
