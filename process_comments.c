#include "shell.h"

/**
 * comment_handler - Processes a command string and removes comments.
 * @cmd: The command line string to be processed.
 *
 * Description: This function searches for the '#' character in the command
 * string. If found and it marks the beginning of a comment, the function
 * truncates the command string at that point,
 * effectively removing the comment.
 * The updated command string is then returned, ready for execution or further
 * processing.
 *
 * Return: The processed command string without comments.
 */
char *comment_handler(char *cmd)
{
	int setoff;
	char *local = str_chr(cmd, '#');

	if (local == NULL)
		return (cmd); /* No comment found, return the original command */

	/* Calculate the index of the '#' character */
	setoff = (&local[0]) - (&cmd[0]);

	/* Check if the '#' is not the start of a comment */
	if (!isspace(cmd[setoff - 1]))
	{
		return (cmd); /* Not a comment, return the original command */
	}
	else
	{
		/* Comment detected, terminate the string before the comment */
		cmd[setoff - 1] = '\0';
	}

	return (cmd);
}

