#include "main.h"

/**
 * dup_str_array - Duplicates the array of strings.
 * @og: The original array of strings to be duplicated.
 *
 * Return: Pointer to the newly allocated duplicate array on success,
 *         or NULL if memory allocation fails.
 */
char **dup_str_array(char **og)
{
	size_t size, index;
	char **dup;

	if (og == NULL)
		return (NULL);

	/* Calculate the size of the original array */
	size = 0;
	while (og[size] != NULL)
		size++;

	/* Allocate memory for the duplicate array */
	dup = malloc((size + 1) * sizeof(char *));
	if (dup == NULL)
		return (NULL);

	/* Duplicate each string in the original array */
	for (index = 0; index < size; index++)
	{
		dup[index] = str_dup(og[index]);
		if (dup[index] == NULL)
		{
			str_free(&dup);
			return (NULL);
		}
	}
	dup[size] = NULL;

	return (dup);
}

/**
 * concat_arrays - Concatenates the contents of one string array to another.
 * @dest: A pointer to the destination string array.
 * @src: The source string array to append.
 */
void concat_arrays(char ***dest, char **src)
{
	int index, src_len = 0, dest_len = 0;

	if (src == NULL)
		return; /* Nothing to concatenate */

	/* Calculate the lengths of the destination and source arrays */
	while ((*dest)[dest_len] != NULL)
		dest_len++;

	while (src[src_len] != NULL)
		src_len++;

	/* Reallocate memory to accommodate the concatenated array */
	*dest = realloc(*dest, (dest_len + src_len + 1) * sizeof(char *));
	if (*dest == NULL)
		return; /* Memory allocation failed */

	/* Copy each string from the source array to the destination array */
	for (index = 0; index < src_len; index++)
	{
		(*dest)[dest_len + index] = str_dup(src[index]);
	}

	/* Null-terminate the concatenated array */
	(*dest)[dest_len + src_len] = NULL;
}

