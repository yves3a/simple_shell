/**
 * builtin_func_handler - Handles the built-in commands.
 * @ptr: Contains all the data relevant to the shell's operation.
 *
 * Description: This function checks the first element of the sub_command array
 * to determine if it matches any of the built-in commands(e.g.env, exit, cd).
 * It then executes the corresponding function.
 *
 * Return: Exit code for the command executed or NOT_BUILTIN if not a built-in.
 */
int builtin_func_handler(shell_t *ptr)
{
	if (!str_cmp(ptr->sub_command[0], "env") ||
		!str_cmp(ptr->sub_command[0], "printenv"))
	{
		env_print();
		return (0);
	}
	else if (!str_cmp(ptr->sub_command[0], "exit"))
	{
		return (exit_handler(ptr, free_multipler));
	}
	else if (!str_cmp(ptr->sub_command[0], "cd"))
	{
		/* Handle the built-in 'cd' command */
		return (cd_handler(ptr));
	}
	else if (!str_cmp(ptr->sub_command[0], "setenv"))
	{
		if (ptr->sub_command[1] && ptr->sub_command[2])
			return (set_env(ptr->sub_command[1], ptr->sub_command[2], 1));
		return (1); /* Invalid number of parameters received */
	}
	else if (!str_cmp(ptr->sub_command[0], "unsetenv"))
	{
		return (unset_env(ptr->sub_command[1]));
	}

	return (NOT_BUILTIN); /* Not a built-in command */
}

