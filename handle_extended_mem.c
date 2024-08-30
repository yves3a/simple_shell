#include "main.h"

/**
 * free_list - Frees a path_t list
 * @head: Pointer to the head node
 */
void free_list(path_t **head)
{
	path_t *cur_node;

	if (head == NULL || *head == NULL)
		return;

	while (*head != NULL)
	{
		cur_node = (*head);
		(*head) = (*head)->next;
		free_safely(cur_node->pathname);
		free_safely(cur_node);
	}
}

/**
 * free_aliases - Frees an alias_t list
 * @head: Pointer to the alias list
 */
void free_aliases(alias_t **head)
{
	alias_t *cur;

	if (*head == NULL || head == NULL)
		return;

	while (*head != NULL)
	{
		cur = (*head);
		(*head) = (*head)->next;
		free_safely(cur->name);
		free_safely(cur->value);
		free_safely(cur);
	}
}

/**
 * multi_free - Frees multiple dynamic memory blocks
 * @format: Specifies the types to free ('s', 't', 'p', 'a')
 *
 * 's' - Single string (char *)
 * 't' - Array of strings (char **)
 * 'p' - path_t list
 * 'a' - alias_t list
 */

void free_multipler(const char *format, ...)
{
	va_list lists;
	char *l;

	va_start(lists, format);

	for (; *format != '\0'; format++)
	{
		switch (*format)
		{
		case 's':
			l = va_arg(lists, char *);
			free_safely(l);
			break;
		case 't':
			str_free(va_arg(lists, char ***));
			break;
		case 'p':
			free_list(va_arg(lists, path_t **));
			break;
		case 'a':
			free_aliases(va_arg(lists, alias_t **));
			break;
		default:
			break;
		}
	}
}

