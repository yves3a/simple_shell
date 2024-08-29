#include "main.h"

/**
 * alias_add - adds an alias to the linked list
 * @aliases: pointer to the list of aliases
 * @izina: alias name to add
 * @val: value associated with the name
 *
 * Return: address of the new alias, or existing one if it already exists
 */
alias_t *alias_add(alias_t **aliases, const char *izina, const char *val)
{
	alias_t *cur, *ali;

	ali = malloc(sizeof(alias_t));
	if (ali == NULL)
		return (NULL);

	ali->name = str_dup(izina);
	ali->value = str_dup(val);
	ali->next = NULL;

	if (*aliases == NULL)
	{
		*aliases = ali;
		return (*aliases);
	}


	/* First for loop to find if the alias already exists */
	for (cur = *aliases; cur != NULL; cur = cur->next)
	{
		if (!str_cmp(izina, cur->name))
		{
			free_safely(cur->value);
			cur->value = str_dup(val);
			free_safely(ali->name);
			free_safely(ali->value);
			free_safely(ali);
			return (cur);
		}
	}

	/* Second for loop to find the last node */
	for (cur = *aliases; cur->next != NULL; cur = cur->next)
	{
		/* Looping until the last node */
	}

	cur->next = ali;

	return (ali);
}

/**
 * aliases_printer - prints all the aliases
 * @ali: list of aliases
 */
void aliases_printer(const alias_t *ali)
{
	if (ali == NULL)
		return;

	for (; ali != NULL; ali = ali->next)
	{
		printf("%s='%s'\n", ali->name, ali->value);
	}
}

/**
 * unalias - removes an alias from the list
 * @aliases: pointer to the list of aliases
 * @command: array of aliases to remove
 *
 * Return: 0 if alias was found and removed, else 1
 */
int unalias(alias_t **ali, char *cmd)
{
	alias_t *cur, *pre;
	size_t i;
	char **names;
	int code_exit = 1;

	names = str_tok(cmd, NULL);
	if (names == NULL)
		return (-1);

	cur = *ali;
	i = 1;
	while (names[i] != NULL)
	{
		for (; cur != NULL; cur = cur->next)
		{
			if (!str_cmp(cur->name, names[i]))
			{
				if (cur == *ali)
					*ali = (*ali)->next;
				else
					pre->next = cur->next;
				free_multipler("ss", cur->name, cur->value);
				free_safely(cur);
				code_exit = 0;
				break;
			}
			pre = cur;
		}
		cur = *ali;

		if (code_exit != 0)
		{
			fprintf(stderr, "unalias: %s not found\n", names[i]);
			code_exit = 1;
		}
		free_safely(names[i]);
		**i++;**
	}
	str_free(&names);
	return (code_exit);
}

/**
 * print_alias - prints a specific alias based on the name given
 * @aliases: list of aliases
 * @name: alias name to print
 *
 * Return: 0 if alias was found, else 1
 */
int alias_printer(const alias_t *ali, const char *name)
{
	if (ali == NULL)
	{
		fprintf(stderr, "alias: %s not found\n", name);
		return (1);
	}

	for (; ali != NULL; ali = ali->next)
	{
		if (!str_cmp(ali->name, name))
		{
			printf("%s='%s'\n", ali->name, ali->value);
			return (0);
		}
	}

	fprintf(stderr, "alias: %s not found\n", name);
	return (1);
}

/**
 * get_alias - returns the value of an alias
 * @aliases: list of aliases
 * @name: alias name
 *
 * Return: alias value, or NULL if not found
 */
char *receive_alias(alias_t *ali, const char *name)
{
	char *val, *chained_val;
	alias_t *cur;

	/* when alias is empty and there is wrong name*/
	if (ali == NULL || name == NULL)
		return (NULL);

	for (cur = ali; cur != NULL; cur = cur->next)
	{
		if (!str_cmp(cur->name, name))
		{
			val = str_dup(cur->value);
			if (val == NULL)
				return (NULL);

			if (!str_cmp(cur->name, cur->value))
				return (val);

			chained_val = receive_alias(ali, val);
			if (chained_val != NULL)
			{
				free_safely(val);
				return (chained_val);
			}
			return (val);
		}
	}
	return (NULL);
}
