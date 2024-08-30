#include "main.h"

/**
 * variables_handler - Expands variables in the shell command.
 * @ptr: Pointer to the shell data structure.
 *
 * Return: Updated command with expanded variables or the original command.
 */
char **variables_handler(shell_t *ptr)
{
	size_t index;
	ssize_t setoff;
	char *val, *local, res[32];

	for (index = 0; ptr->sub_command[index] != NULL; index++)
	{
		local = str_chr(ptr->sub_command[index], '$');
		if (local == NULL)
			continue; /* No variable found, continue */

		setoff = (&local[0]) - (&ptr->sub_command[index][0]);

		if (ptr->sub_command[index][setoff + 1] == '\0')
			continue; /* Ignore literal '$' */

		if (ptr->sub_command[index][setoff + 1] == '$')
		{
			_itoa(getpid(), res);
			free_safely(ptr->sub_command[index]);
			ptr->sub_command[index] = str_dup(res); /* Replace with PID */
		}
		else if (ptr->sub_command[index][setoff + 1] == '?')
		{
			_itoa(ptr->code_exiter, res);
			free_safely(ptr->sub_command[index]);
			ptr->sub_command[index] = str_dup(res); /* Replace with exit code */
		}
		else
		{
			val = get_env(local + 1);
			free_safely(ptr->sub_command[index]);
			ptr->sub_command[index] = str_dup(val); /* Replace with env variable */
		}
	}

	return (ptr->sub_command);
}

