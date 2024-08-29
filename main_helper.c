#include "shell.h"
#include _MAIN_H_

/**
 * shell_init - Allocates and initializes the shell data structure
 *
 * Return: Pointer to the initialized shell data structure
 */
shell_t *shell_init(void)
{
	shell_t *ptr = malloc(sizeof(shell_t));

	if (ptr == NULL)
	{
		/* Allocation failed, print error message and exit */
		fprintf(stderr, "Launching shell failed.\n"
				"Please ensure you have enough system resources for this operation\n");
		exit(-1);
	}

	/* Initialize all members of the shell structure to their default values */
	ptr->listof_path = NULL;
	ptr->aliases = NULL;
	ptr->line = NULL;
	ptr->sub_command = NULL;
	ptr->commands = NULL;
	ptr->tokens = NULL;
	ptr->token = NULL;
	ptr->counter_of_cmd = 0;
	ptr->code_exiter = 0;

	return (ptr);
}

/**
 * return_hostname -Retrieves the hostname of the system
 * from the '/etc/hostname' file
 * @buffer: The buffer to store the hostname;
 *		should be large enough to hold the hostname
 *
 * Return: Pointer to the buffer containing the hostname,
 *	or "ptr" if an error occurs.
 */
char *return_hostname(char *buffer)
{
	int file_descr, n_read;

	file_descr = open("/etc/hostname", O_RDONLY);

	/* Check if opening the file failed */
	if (file_descr == -1)
	{
		/* If opening failed, use 'ptr' as the default hostname */
		_strcpy(buffer, "ptr");
		return (buffer);
	}

	/* Successfully opened the file, now read the hostname */
	num_read = read(file_descr, buffer, 100);

	/*Check if reading failed or no data was read, fall back to 'msh' if needed*/
	if (num_read == -1 || num_read == 0)
		_strcpy(buffer, "ptr");
	else
	/* Successfully read the hostname, remove the trailing newline */
		buffer[num_read - 1] = '\0';

	close(file_descr);

	return (buffer);
}
/**
 * prompt_shower - Displays the shell prompt in interactive mode
 */
void prompt_shower(void)
{
	char prompt[PROMPT_SIZE], name_of_host[100];
	char *name_of_user = get_env("USER"), *pwd;

	if (name_of_user != NULL)
	{
		pwd = get_env("PWD"); /*Get the current working directory*/
		if (pwd != NULL)
		{
			/* Display only the last part of the directory in the prompt */
			pwd = (*pwd == '/' && *(pwd + 1) == '\0')
					  ? pwd
					  : (str_r_chr(pwd, '/') +
						 1); /* show only the current directory */
		/* Format the prompt with username, hostname, and current directory */
			sprintf(prompt, "[%s@%s %s]%% ", name_of_user,
				return_hostname(name_of_host),
				(!_strcmp(pwd, name_of_user))
						? "~" /* Display '~' for the user's home directory */
						: pwd);
		}
	}
	else
	{
	/* If environment variables are insufficient, fall back to a minimal prompt */
		sprintf(prompt, "msh%% ");
	}

	/* Display the prompt only in interactive mode */
	if (isatty(STDIN_FILENO))
		printf("%s", prompt);
}

/**
 * handle_signalint- Custom handler for SIGINT (Ctrl+C)
 * @signum: ignal number (unused in this handler)
 */
void handle_signalint(__attribute__((unused))int signum)
{
	putchar('\n');
	prompt_shower();
	fflush(stdout);
}
