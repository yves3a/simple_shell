#include "shell.h"

/**
 * set_env - Set or update an environment variable
 * @name: Name of the variable
 * @val: Value to be set
 * @over_write: Overwrite if the exists
 *
 * Return: 0 on success, 1 on error
 */
int set_env(const char *name, const char *val, int over_write)
{
	size_t len, index;
	char *var_of_env = NULL;

	/* Validate variable name */
	if (name == NULL || name[0] == '\0' || str_chr(name, '=') != NULL)
	{
		fprintf(stderr, "Invalid variable name: %s\n", name);
		return (1);
	}
	/* Check if the variable exists */
	if (get_env(name) != NULL)
	{
		if (over_write)
		{
			if (unset_env(name) != 0)
				return (1);
		}
		else
			return (0); /* Variable exists, and overwrite is false */
	}

	/* Calculate the length and allocate memory for the new variable */
	len = str_len(name) + str_len(val) + 2;
	var_of_env = malloc(len);
	if (var_of_env == NULL)
		return (1);

	sprintf(var_of_env, "%s=%s", name, val);

	/* Check if the variable is already in the environment and update it */
	for (index = 0; environ[index] != NULL; index++)
	{
		if (strn_cmp(environ[index], name, len) == 0 && environ[index][len] == '=')
		{
			environ[index] = var_of_env;
			safe_free(var_of_env);
			return (0);
		}
	}
	/* Add the new variable to the environment */
	environ[index++] = var_of_env;
	environ[index] = NULL;
	safe_free(var_of_env);

	return (0);
}

/**
 * unset_env - Unset an environment variable
 * @name: The name of the variable to unset
 *
 * Return: 0 on success, 1 on error
 */
int unset_env(const char *name)
{
	size_t index, j, len;
	/* Validate variable name */
	if (name == NULL || name[0] == '\0' || str_chr(name, '=') != NULL)
	{
		fprintf(stderr, "Invalid variable name: %s\n", name);
		return (1);
	}

	len = str_len(name);

	/* Search for the variable in the environment and remove it */
	for (index = 0; environ[index] != NULL; index++)
	{
		if (strn_cmp(environ[index], name, len) == 0 && environ[index][len] == '=')
		{
			/* Shift variables to fill the gap */
			for (j = index; environ[j] != NULL; j++)
				environ[j] = environ[j + 1];
			return (0);
		}
	}

	/* Variable not found */
	dprintf(STDERR_FILENO, "Variable not found: %s\n", name);
	return (1);
}

/**
 * exit_handler - Handles the built-in `exit` command for the shell
 * @ptr: Shell's context and data
 * @cleanup: cleanup function
 *
 * Return: 2 on error, else exits with the provided exit code
 */
int exit_handler(s_shell_t *ptr, void (*cleanup)(const char *format, ...))
{
	const char *code_status = (ptr->sub_command) ? ptr->sub_command[1] : NULL;
	int code_exitor = ptr->code_exiter;

	if (code_status == NULL)
	{
		cleanup("spattt", ptr->line, &ptr->listof_path, &ptr->aliases,
				&ptr->commands, &ptr->sub_command, &ptr->tokens);
		safe_free(ptr);
		exit(code_exitor);
	}

	if (isalpha(*code_status) || _atoi(code_status) < 0 || *code_status == '-')
	{
		dprintf(STDERR_FILENO, "%s: %lu: exit: Illegal number: %s\n",
				ptr->program_name, ptr->counter_of_cmd, code_status);
		return (CMD_ERR);
	}

	code_exitor = _atoi(code_status);
	cleanup("spattt", ptr->line, &ptr->listof_path, &ptr->aliases,
			&ptr->commands, &ptr->sub_command, &ptr->tokens);
	safe_free(ptr);
	exit(code_exitor);
}

/**
 * cd_handler - Handles the `cd` command
 * @ptr: Shell's context and data
 *
 * Return: 0 on success, else 2 on error
 */
int cd_handler(s_shell_t *ptr)
{
	char path[PATH_SIZE], print_work_dir[BUFF_SIZE];
	const char *name_of_path = ptr->sub_command[1];
	char *home = get_env("HOME"), *old_path = get_env("OLDPWD");

	getcwd(print_work_dir, BUFF_SIZE);
	oldpath = (old_path) ? old_path : print_work_dir;
	if (name_of_path != NULL && *name_of_path != '~')
	{
		int d_dash = !str_cmp(name_of_path, "-") || !str_cmp(name_of_path, "--");

		if (!str_chr(name_of_path, '/') && !d_dash)
			sprintf(path, "%s/%s", print_work_dir, ((d_dash) ? old_path : name_of_path));
		else
			sprintf(path, "%s", ((d_dash) ? old_path : name_of_path));
		if (chdir(path) == -1)
		{
			if (str_spn(name_of_path, "-") > 2)
				fprintf(stderr, "%s: %lu: cd: Illegal option: --\n", ptr->program_name,
						ptr->counter_of_cmd);
			else
				fprintf(stderr, "%s: %lu: cd: can't cd to %s\n", ptr->program_name,
						ptr->counter_of_cmd, name_of_path);
			return (CMD_ERR);
		}
		if (d_dash)
			printf("%s\n", old_path);
		set_env("OLDPWD", print_work_dir, 1);
		getcwd(path, PATH_SIZE);
		set_env("PWD", path, 1);
	}
	else
	{
		if (home == NULL)
			return (0);
		if (chdir(home) == -1)
			return (CMD_ERR);
		set_env("OLDPWD", print_work_dir, 1);
		set_env("PWD", home, 1);
	}
	return (0);
}

/**
 * env_print - Prints the environment variable
 *
 * Description: it prints the env variable of the
 * current scenario
 */
void env_print(void)
{
	int file_d, stat;
	char *name_of_file = "/tmp/env";
	pid_t child;

	/* opens a file for read,write, create and trunc mode*/
	file_d = open(name_file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (file_d == -1)
		return;

	if (write(file_d, "env", 3) != -1)
	{
		char *argv[] = {"/bin/sh", "/tmp/env", NULL};

		child = fork();
		if (child == 0)
			execve(argv[0], argv, environ);
		else
			waitpid(child, &stat, 0);
		close(file_d);
	}
	else
	{
		close(file_d);/* it closes the file descriptor*/
		return;
	}
}

