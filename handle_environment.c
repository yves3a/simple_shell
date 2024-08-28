#include "shell.h"

/**
 * get_env - Retrieves the value of an environment variable.
 * @name: Name of the environment variable.
 *
 * Return: Pointer to the variable's value, or NULL if not found.
 */
char *get_env(const char *name)
{
	int index;
	size_t len = str_len(name);

	for (index = 0; environ[index] != NULL; index++)
	{
		/* Check for a matching variable */
		if (_strstr(environ[index], name) && environ[index][len] == '=')
		{
			/* Return the value after '=' */
			return ((_strchr(environ[index], '=')) + 1);
		}
	}

	return (NULL);
}

/**
 * path_builder - Creates a linked list of directories in PATH.
 * @head: Pointer to the head of the list.
 *
 * Return: Pointer to the head on success, or NULL on failure.
 */
path_file_t *path_builder(path.file_t **head);
{
	size_t index = 0;
	char *value_of_path = NULL, **names_of_path = NULL;

	path_file_t *new_node = NULL, *tail = NULL;

	value_of_path = get_env("PATH");
	if (value_of_path == NULL || *value_of_path == '\0')
		return (NULL); /* PATH is not set */

	names_of_path = _strtok(value_of_path, ":");
	if (names_of_path == NULL)
		return (NULL); /* Couldn't tokenize PATH */
	while (names_of_path[index] != NULL)
	{
		new_node = malloc(sizeof(path.file_t));
		if (new_node == NULL)
			return (NULL);

		new_node->pathname = str_dup(names_of_path[index]);
		str_cpy(new_node->pathname, names_of_path[index]);
		new_node->next = NULL;

		if (*head == NULL)
		{
			(*head) = new_node;
		}
		else
		{
			tail = *head;
			while (tail->next != NULL)
				tail = tail->next;

			tail->next = new_node;
		}
		/* Free memory for the token */
		safe_free(names_of_path[index]);
		index++;
	}

	free_str(&names_of_path);
	return (*head);
}

/**
 * path_printer - Prints the directories in the PATH list.
 * @list: list of pathnames
 */
void path_printer(path.file_t *list)
{
	if (list == NULL)
		return;

	while (list != NULL)
	{
		printf("%s\n", list->pathname);
		list = list->next;
	}
}
