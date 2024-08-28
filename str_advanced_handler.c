#include "s_shell.h"

/**
 * str_tok - tokenizes a string into words using a delimiter.
 * @str: The string to be splitted.
 * @delim: The delimiter used to split strings.
 *
 * Return: On sucess, pointer to an array of strings ,
 *         NULL on failure.
 */
char **str_tok(const char *str, const char *delim)
{
	char **array_of_str;
	int count_word, starting_word, ending_word; /* Tracks the position of words */
	int len, index, i;
	char *cur_delim = (char *)delim;

	if (delim == NULL)
		cur_delim = " \t\n"; /* Default delimiters: space, tab, newline */

	count_word = get_count_word(str, cur_delim);
	if (count_word == 0)
		return (NULL); /* No valid words found */

	array_of_str = malloc((count_word + 1) * sizeof(char *));
	if (array_str == NULL)
		return (NULL); /* Memory allocation failed */

	starting_word = index = 0;
	len = str_len(str);

	for (i = 0; i < len; i++)
	{
		if ((!str_chr(cur_delim, str[i]) && str_chr(cur_delim, str[i + 1])) ||
				(!str_chr(cur_delim, str[i]) && str[i + 1] == '\0'))
		{
			ending_word = i + 1;
			array_of_str[index] = new_ijambo(str, starting_word, ending_word);

			if (array_of_str[index] == NULL)
				return (str_free(&array_of_str), NULL); /* Memory allocation failed */
			index++;
		}
		else if (!str_chr(cur_delim, str[i]) && !str_chr(cur_delim, str[i + 1]))
			continue; /* Still in a word, continue */
		else
			starting_word = i + 1;
	}
	array_of_str[index] = NULL; /* Terminate the array */
	return (array_of_str);
}

/**
 * strp_brk - Searches a string for any set of bytes.
 * @str: The string to search.
 * @accept: The substring containing bytes to match.
 *
 * Return: Pointer to the byte in @s that matches one in @accept,
 *         or NULL if no such byte is found.
 */
char *strp_brk(const char *str, const char *accept)
{
	int index, j;

	for (index = 0; str[index] != '\0'; index++)
	{
		for (j = 0; accept[j]; j++)
		{
			if (str[index] == accept[j])
				return ((char *)&str[index]); /* Match found */
		}
	}
	return (NULL); /* No match found */
}

/**
 * str_spn - Gets the length of a prefix substring.
 * @str: The main string.
 * @accept: The substring containing acceptable bytes.
 *
 * Return: Number of bytes in the initial segment of @s consisting
 *         only of bytes from @accept.
 */
size_t str_spn(const char *str, const char *accept)
{
	size_t len = 0;
	int index, map[256] = {0}; /* Hash map for efficient lookup */

	if (str == NULL || accept == NULL)
		return (0); /* Handle NULL input */

	for (index = 0; accept[index] != '\0'; index++)
		map[(unsigned char)accept[index]] = 1; /* Map bytes from accept */

	for (index = 0; str[index] != '\0'; index++)
	{
		if (str[index] == SPACE)
			break; /* Stop at the next word */
		else if (map[(unsigned char)str[index]])
			len++; /* Match found, increment length */
		else
			break; /* No match found */
	}
	return (len);
}

/**
 * str_r_chr - Locates the last occurrence of a character in a string.
 * @str: The string to search.
 * @c: The character to find.
 *
 * Return: Pointer to the last occurrence of @c in @s, or NULL if
 *         not found.
 */
char *str_r_chr(const char *str, int c)
{
	char *tmp_str = (char *)str;
	size_t len = str_len(tmp_str);

	if (str == NULL)
		return (NULL); /* Handle NULL input */

	while (tmp_str[--len] != '\0')
	{
		if (tmp_str[len] == c)
			return (tmp_str + len); /* Match found */
	}

	if (c == '\0')
		return (tmp_str + len); /* Return pointer to null byte */

	return (NULL); /* No match found */
}

/**
 * get_count_word - Counts the number of words in a string.
 * @str: The string to evaluate.
 * @delim: The delimiter used to split the words.
 *
 * Return: Number of words in the string.
 */
int get_count_word(const char *str, const char *delim)
{
	int within_word = 0; /* Tracks if currently in a word */
	int word_counter = 0;
	char *cur_delim = (char *)delim;

	if (str == NULL || *str == '\0')
		return (0); /* Handle NULL or empty string */

	if (delim == NULL)
		cur_delim = " \t"; /* Default delimiters: space, tab */

	while (*str)
	{
		if (str_chr(cur_delim, *str))
		{
			within_word = 0; /* Not in a word, reset flag */
		}
		else if (!within_word)
		{
			within_word = 1;  /* Start of a new word */
			word_counter++; /* Increment word count */
		}
		str++;
	}

	return (word_counter);
}

