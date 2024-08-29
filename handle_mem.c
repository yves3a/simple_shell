#include "main.h"

/**
 * _realloc - reallocate dynamic memory
 * @old_mem_blk: pointer to the old memory block
 * @old_size: size of the old memory block
 * @new_size: size of the new memory block
 *
 * Descrition: Description: Resizes a memory block pointed
 * to by @old_block to @new_size bytes. If @new_size is larger,
 * the extra memory is uninitialized. Acts as malloc
 * if @old_block is NULL. Acts as free if @new_size
 * is zero and @old_block is not NULL
 *
 * Return: a pointer to the newly allocated memory,
 * or NULL if the request failed
 */
void *_realloc(void *old_block, size_t old_size, size_t new_size)
{
	void *new_block;
	size_t copy_size;

	if (new_size == old_size)
		return (old_block); /* nothing to do, old and new sizes are the same */

	/* handle the free() equivalent call of the _realloc function */
	if (new_size == 0 && old_block != NULL)
	{
		free_safely(old_block);
		return (NULL);
	}

	new_block = malloc(new_size);
	if (new_block == NULL)
		return (NULL); /* memory allocation failed */

	if (old_block != NULL)
	{
		copy_size = (old_size < new_size) ? old_size : new_size;
		/* copy the data from old memory block to new memory block */
		_memcpy(new_block, old_block, copy_size);

		free_safely(old_block); /* free old allocated memory block */
	}

	return (new_block);
}

/**
 * *_memcpy - copies memory area
 * @dest: memory area
 * @src: source
 * @n: length of src
 *
 * Return: pointer to dest.
 */
char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
	{
		dest[i] = src[i];
	}
	return (dest);
}

/**
 * new_word - gets a word a from string and writes it to a memory buffer
 * @str: string to copy from
 * @start: start index of the new word in the string
 * @end: end index of word
 *
 * Return: a pointer to the new word
 */
char *new_word(const char *str, int start, int end)
{
	char *new_word;
	int word_len = end - start;

	new_word = malloc(sizeof(char) * (word_len + 1));
	if (new_word == NULL)
	{
		return (NULL); /* memory allocation failed */
	}

	_strncpy(new_word, str + start, word_len);
	new_word[word_len] = '\0';

	return (new_word);
}

/**
 * _free - a safer way to free dynamically allocated memory
 * @ptr: pointer to memory location
 *
 * Description: This _free() function takes care of freeing
 * dynamically allocated memory while ensuring the pointer
 * @ptr passed to it is not NULL before trying to free it.
 * Also, after freeing the memory, it sets the pointer @ptr
 * to NULL to avoid the issue of dangling pointers
 */
void _free(void **ptr)
{
	if (ptr != NULL && *ptr != NULL)
	{
		free(*ptr);
		*ptr = NULL;
	}
}

/**
 * free_str - frees memory allocated for an array of strings
 * @str_array: string array
 */
void free_str(char ***str_array)
{
	int i = 0;

	if (str_array == NULL || (*str_array) == NULL)
		return; /* there's nothing to free */

	/* free memory allocated for each string */
	for (i = 0; (*str_array)[i] != NULL; i++)
	{
		safe_free((*str_array)[i]);
	}

	if ((*str_array) != NULL)
		safe_free((*str_array));
}
