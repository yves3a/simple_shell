#include "shell.h"
#include "main.h"

/**
 * strn_cmp - Compare at most n bytes of two strings.
 * @str1: The first string to be compared.
 * @str2: The second string to be compared.
 * @n: The maximum number of bytes to compare.
 *
 * Return: 0 if the strings are equal, a negative value if s1 is less than s2,
 *         or a positive value if s1 is greater than s2.
 */
int strn_cmp(const char *str1, const char *str2, size_t n)
{
	size_t index = 0;

	while (*str1 == *str2 && index < n)
	{
		if (*str1 == '\0' || index + 1 == n)
		{
		/* Strings are equal or when reached the end of comparison */
			return (0);
		}
		str1++;
		str2++;
		index++;
	}

	return ((*str1) - (*str2));
}

/**
 * str_chr - Locate a character in a string.
 * @str: The string to search.
 * @c: The character to locate.
 *
 * Description: This function returns a pointer to the first occurrence of
 * the character @c in the string @str.
 *
 * Return: A pointer to the matched character or NULL if the character
 *         is not found.
 */
char *str_chr(const char *str, int c)
{
	char *tmp_str = (char *)str;

	/* Handle empty string */
	if (str == NULL)
		return (NULL);

	while (*tmp_str != '\0')
	{
		if (*tmp_str == c)
			return (tmp_str); /* Match found */
		tmp_str++;
	}

	/* Return a pointer to the null byte if 'c' is a null byte. */
	if (c == '\0')
		return (tmp_str);

	return (NULL); /* No match found, return NULL */
}

/**
 * str_str - Locate a substring in a string.
 * @str: The string to search from.
 * @sub_str: a substring to be located.
 *
 * Description: This function finds the first occurrence of the substring
 * @substr in the string @str. The terminating null bytes ('\0') are
 * not compared.
 *
 * Return: A pointer to the beginning of the located substring,
 *         or NULL if the substring is not found.
 */
char *str_str(const char *str, const char *sub_str)
{
	size_t index, j;

	for (index = 0; str[index]; index++)
	{
		j = 0;

		/* Continue searching while characters in both strings match */
		while (str[index + j] == sub_str[j] && sub_str[j])
			j++;

		/* End of substring reached, indicating a full match */
		if (sub_str[j] == '\0')
		{
			return ((char *)&str[index]);
		}
	}

	return (NULL); /* No match found, return NULL */
}

/**
 * str_dup - Duplicate a string.
 * @str: The string to duplicate.
 *
 * Description: This function returns a pointer to a newly allocated
 * space in memory, and it contains a copy of the string @str.
 *
 * Return: A pointer to the duplicated string, or NULL if it fails.
 */
char *str_dup(const char *str)
{
	char *str_dup;
	size_t index = 0;

	if (str == NULL)
		return (NULL); /* Handle NULL string */

	str_dup = malloc(str_len(str) + 1);

	if (str_dup == NULL)
		return (NULL);

	/* Copy the string into the newly allocated memory */
	while (str[index])
	{
		str_dup[index] = str[index];
		index++;
	}
	str_dup[index] = '\0';

	return (str_dup);
}

/**
 * strn_cpy - Copy a string up to a specified number of bytes.
 * @dest: The destination string buffer.
 * @src: The source string buffer.
 * @n: The number of bytes to copy.
 *
 * Description: This function copies up to n bytes from the string src to
 * dest. If there is no null byte among the first @n bytes of src, the string
 * in dest will not be null-terminated.
 *
 * Return: A pointer to the destination string @dest.
 */
char *strn_cpy(char *dest, const char *src, size_t n)
{
	size_t index;

	for (index = 0; index < n && src[index] != '\0'; index++)
	{
		dest[index] = src[index];
	}

	while (index < n)
	{
		/* Ensure that n bytes are written only */
		dest[index] = '\0';
		index++;
	}

	return (dest);
}

