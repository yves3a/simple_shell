#include "main.h"

/**
 * str_cmp - compares two strings
 * @s1: first string
 * @s2: second string
 *
 * Return: 0 if equal, negative if s1 < s2, positive if s1 > s2
 */
int str_cmp(const char *s1, const char *s2)

	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

/**
 * str_cat - concatenates two strings
 * @dest: destination buffer where to append
 * @src: source string to be appended
 *
 * Return: pointer to the resulting string dest
 */
char *str_cat(char *dest, const char *src)
{
	int leng, index;

	leng = str_len(dest);

	for (index = 0; src[index] != '\0'; index++)
		dest[leng++] = src[index];
	dest[leng] = '\0';

	return (dest);
}

/**
 * str_cpy - copies a string
 * @dest: a string where to paste
 * @src: source string
 *
 * Return: pointer to the destination string dest
 */
char *str_cpy(char *dest, const char *src)
{
	size_t index = 0;

	while (src[index] != '\0')
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';

	return (dest);
}

/**
 * str_len - gets the string lenght
 * @s: a string
 *
 * Return: length of the string
 */
size_t str_len(const char *str)
{
	int index = 0;

	if (str == NULL)
		return (0);

	while (str[index] != '\0')
		index++;

	return (index);
}

/**
 * rec_operator - finds logical operators in a string
 * @str: the string to search
 *
 * Return: "&&" or "||" if found, else NULL
 */
char *rec_operator(char *str)
{
	char *oper = NULL;
	int index;

	if (str == NULL)
		return (NULL);

	for (index = 0; str[index] != '\0'; index++)
	{
		if (str[index] == '&' && str[index + 1] == '&')
		{
			oper = "&&";
			break;
		}
		else if (str[index] == '|' && str[index + 1] == '|')
		{
			oper = "||";
			break;
		}
	}

	return (oper);
}

