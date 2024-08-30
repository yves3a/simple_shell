#include "main.h"

/**
 * line_parsing - Parses the received command line.
 * @ptr: Contains all the data relevant to the shell's operation.
 *
 * Description: This function processes
 * the command line before handing it
 * over to the executor.
 * It skips empty lines and comments,
 * handles tokenization,
 * and calls the executor if the command is valid.
 *
 * Return: The exit code of the executed program, or -1 if an error occurs.
 */
int line_parsing(shell_t *ptr)
{
	size_t index;

	/* Skip empty lines or lines starting with a comment */
	if (*ptr->line == '\n' || *ptr->line == '#')
		return (0);

	/* Remove comments from the command line */
	ptr->line = comment_handler(ptr->line);

	/* Tokenize the command line */
	ptr->tokens = str_tok(ptr->line, "\n");
	if (ptr->tokens == NULL)
	{
		fprintf(stderr, "Not enough system memory to continue\n");
		return (-1);
	}

	/* Process each tokenized command */
	for (index = 0; ptr->tokens[index] != NULL; index++)
	{
		ptr->token = ptr->tokens[index];

		/* Handle the 'exit' command if provided */
		if (!str_cmp(ptr->tokens[index], "exit") && ptr->tokens[index + 1] == NULL)
		{
			exit_handler(ptr, free_multipler);
		}

		/* Tokenize and execute commands separated by ';' */
		ptr->commands = str_tok(ptr->token, ";\n");
		if (ptr->commands == NULL)
		{
			fprintf(stderr, "Memory allocation failed...\n");
			return (-1);
		}

		/* Parse and execute the command */
		ptr->code_exiter = parsing(ptr);
		str_free(&ptr->commands);
	}

	str_free(&ptr->tokens);
	return (ptr->code_exiter);
}

/**
 * parsing - Parses and executes an array of commands.
 * @ptr: Contains all the data relevant to the shell's operation.
 *
 * Description: This function iterates through the array of commands,
 * processes operators (&&, ||), and delegates
 * the execution to helper functions.
 *
 * Return: The exit code of the executed program.
 */
int parsing(shell_t *ptr)
{
	ssize_t index, set_off;
	char *cur_cmd = NULL, *oper = NULL;
	char *cmd_next = NULL, *next_cmd_temp = NULL;

	for (index = 0; ptr->commands[index] != NULL; index++)
	{
		oper = rec_operator(ptr->commands[index]);
		if (oper != NULL)
		{
			set_off = str_spn(ptr->commands[index], oper);
			/* Extract the command before the operator */
			cur_cmd = strndup(ptr->commands[index], set_off);
			if (cur_cmd == NULL)
				return (0);
			ptr->sub_command = str_tok(cur_cmd, NULL);
			free_safely(cur_cmd);
			if (ptr->sub_command == NULL)
				return (0);
			ptr->sub_command = variables_handler(ptr);
			parsing_helper(ptr, index);

			/* Prepare the next command after the operator */
			next_cmd_temp = str_dup(&ptr->commands[index][set_off + 2]);
			free_safely(cmd_next);
			free_safely(ptr->commands[index]);

			/* Check the exit code and determine the next command to execute */
			if ((!str_cmp(oper, "&&") && ptr->code_exiter == 0) ||
					(!str_cmp(oper, "||") && ptr->code_exiter != 0))
			{
				ptr->commands[index] = cmd_next;
				parsing(ptr);
				cmd_next = next_cmd_temp;
			}
			else
				free_safely(next_cmd_temp);
		}
		else
			parsing_and_execute(ptr, index);
	}
	return (ptr->code_exiter);
}

/**
 * parsing_and_execute - Parses and executes a single command.
 * @ptr: Contains all the data relevant to the shell's operation.
 * @i: The current index in the commands array.
 *
 * Description: This function tokenizes the sub-commands and processes
 * variables before executing them. It handles any errors encountered during
 * execution.
 *
 * Return: The exit code of the executed program.
 */
int parsing_and_execute(shell_t *ptr, size_t i)
{
	/* Tokenize the sub-commands */
	ptr->sub_command = str_tok(ptr->commands[i], NULL);
	if (ptr->sub_command == NULL)
	{
		return (0); /* Skip empty commands or lines with just whitespace */
	}

	/* Process variables in the command */
	ptr->sub_command = variables_handler(ptr);
	if (ptr->sub_command[0] != NULL && ptr->sub_command != NULL)
		parsing_helper(ptr, i);
	else
		str_free(&ptr->sub_command);

	/* Cleanup and return the exit code */
	free_safely(ptr->commands[i]);
	return (ptr->code_exiter);
}

/**
 * parsing_helper - Assists in parsing and executing commands.
 * @ptr: Contains all the data relevant to the shell's operation.
 * @i: The current index in the commands array.
 *
 * Description: This function handles aliases, built-in commands, and
 * commands executed with the PATH variable. It is called by parse and
 * parse_and_execute to aid in processing commands.
 */
void parsing_helper(shell_t *ptr, size_t i)
{
	char *value_of_alias;

	/* Handle alias and unalias commands */
	if (!str_cmp(ptr->sub_command[0], "alias") ||
			!str_cmp(ptr->sub_command[0], "unalias"))
	{
		ptr->code_exiter = alias_handler(&ptr->aliases, ptr->commands[i]);
		str_free(&ptr->sub_command);
		return;
	}

	/* Expand alias if one exists */
	value_of_alias = receive_alias(ptr->aliases, ptr->sub_command[0]);
	if (value_of_alias != NULL)
	{
		cmd_alias_builder(&ptr->sub_command, value_of_alias);
		free_safely(value_of_alias);
	}

	/* Execute built-in commands */
	ptr->code_exiter = builtin_handler(ptr);
	if (ptr->code_exiter != NOT_BUILTIN)
	{
		str_free(&ptr->sub_command);
		return; /* Built-in command executed successfully */
	}

	/* Handle commands with the PATH variable */
	if (ptr->listof_path != NULL && !str_chr(ptr->sub_command[0], '/'))
	{
		ptr->code_exiter = handler_of_path(ptr);
		if (ptr->code_exiter == -1)
			ptr->code_exiter = print_command_not_found(ptr);
	}
	else
	{
		/* Handle commands with absolute or relative paths */
		if (access(ptr->sub_command[0], X_OK) == 0 &&
				str_chr(ptr->sub_command[0], '/'))
			ptr->code_exiter = command_executor(ptr->sub_command[0], ptr);
		else
			ptr->code_exiter = print_command_not_found(ptr);
	}
	str_free(&ptr->sub_command);
}

/**
 * print_command_not_found - Prints an error when a command is not found.
 * @ptr: Contains all the data relevant to the shell's operation.
 *
 * Description: This function prints a "command not found" error message to
 * the standard error stream.
 *
 * Return: 127 if the command is not found, else 0.
 */
int print_command_not_found(shell_t *ptr)
{
	dprintf(STDERR_FILENO, "%s: %lu: %s: not found\n", ptr->program_name,
			ptr->counter_of_cmd, ptr->sub_command[0]);

	return (CMD_NOT_FOUND);
	/*
	 * Command not found
	 */
}

