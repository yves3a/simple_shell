#include "s_shell.h"

/**
 * command_executor - Execute a command
 * @pathname: Absolute path to the binary file
 * @ptr: Shell context
 *
 * Return: 0 on success, -1 on failure
 */
int command_executor(const char *pathname, s_shell_t *ptr)
{
	int stat;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
	{
		/* Execute command in child process */
		if (execve(pathname, ptr->sub_command, environ) == -1)
		{
			if (errno == EACCES)
			{
				fprintf(stderr, "%s: %lu: %s\n", ptr->program_name,
						++ptr->counter_of_cmd, strerror(errno));
				return (126);
			}
			perror("execve");
			return (-1);
		}
	}
	else
	{
		/* Parent process waits for child to finish */
		if (waitpid(pid, &stat, 0) == -1)
		{
			perror("wait");
			return (-1);
		}
		if (WIFEXITED(stat))
		{
			return (WEXITSTATUS(stat));
		}
	}

	return (0);
}
