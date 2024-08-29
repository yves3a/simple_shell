#include "main.h"

/**
 * main -Entry point for the shell program
 * @argc: Number of command line arguments
 *
 * @argv: Array of command line arguments
 *
 * Return: 0 on success, or the exit code of
 * the last executed command.
 */

int main(int argc, char *argv[])
{
	size_t lenght = 0;
	ssize_t read_num = 0;
	shell_t *ptr = NULL;

	/*Handle signal interrupts (e.g:Ctrl + C) by setting a custom signal handler*/
	signal(SIGINT, handle_signalint);

	/*Initialize the shell structure and set the program name*/
	ptr = shell_init();
	ptr->program_name = argv[0];
	path_builder(&ptr->listof_path);

	/**
	 * If a file is passed as an argument, treat it as input
	 * and execute commands from it
	 */
	if (argc >= 2)
	{
		file_as_input_handler(argv[1], ptr);
	}

	/*Main loop: continues running the shell until an exit condition is met*/

	while (RUNNING)
	{
		prompt_shower();/*display the shell prompt*/
		fflush(stdout);/*Ensure the prompt is displayed immediately*/

		/*Read the next command line input*/

		read_num = _getline(&ptr->line, &lenght, STDIN_FILENO);

		/* Increment the command counter for each input received */
		++ptr->counter_of_cmd;

		/* check for empty prompt or if Ctrl+D to signal end of input*/
		if (read_num == 0)
		{
			if (isatty(STDIN_FILENO))
				printf("exit\n");
exit_handler(ptr, free_multipler);
/*
 * clean up and
 * exit the shell
 */
}
/*
 * Parse the input line
 * and
 * execute the command
 */
ptr->code_exiter = line_parsing(ptr);
safe_free(ptr->line);
/*Free the memory allocated for the inputs*/
}
/* Returns the exit code from the lasts executed commands */
	return (ptr->code_exiter);
}
