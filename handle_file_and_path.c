#include "shell.h"

/**
 * handler_of_path - Executes a command when
 * the PATH environment variable is set.
 * @ptr: Pointer to the shell structure containing relevant shell data.
 *
 * Return: The exit code of the child process if
 * the command is found and executed,
 *	otherwise -1 if the command is not found in the PATH.
 */

int handler_of_path(shell_t *ptr);
{
	char path[BUFF_SIZE];
	path_shell_t *listof_path = ptr->listof_path;

	while (listof_path != NULL)
	{
		sprintf(path, "%s%s%s", listof_path->pathname, "/", ptr->sub_command[0]);
		if (access(path, X_OK) == 0)
		{
			return (command_executor(path, ptr));
		}
		else if (access(ptr->sub_command[0], X_OK) == 0)
		{
			return (command_executor(path, ptr));
		}
		listof_path = listof_path->next;
	}

	return (-1);
}

/**
 * file_as_input_handler -Handles shell execution
 * in non-interactive mode using a file.
 * @filename: Name of the file to read commands from.
 * @ptr: Pointer to the shell structure containing relevant shell data.
 *
 * Return: 0 on success, or the exit status of the last executed process.
 */

void file_as_input_handler(const char *filename, shell_t *ptr);
{
	size_t num = 0;
	int num_read, fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		/* File could not be opened, clean up and exit */
		free_list(&ptr->listof_path);
		fprintf(stderr, "%s: 0: Can't open %s\n", ptr->program_name, filename);
		exit(CMD_NOT_FOUND);
	}

	num_read = _getline(&ptr->line, &num, fd);


	/* Report error if file descriptor cannot be closed*/
	 /* but continue with the remaining operations.*/
	if (close(fd) == -1)
		fprintf(stderr, "An error occurred while closing file descriptor #%d\n", fd);

	if (num_read == -1)
	{
		ptr->code_exiter = -1;
		exit_handler(ptr, free_multipler);
	}

	if (n_read)
	{
		ptr->program_name = filename;
		line_parsing(ptr);
	}

	exit_handler(ptr, free_multipler);
}
