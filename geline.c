#include "main.h"

/**
 * _getline - reads input from a file descriptor
 * @line_ptr: a pointer to the string buffer
 * @n: number of bytes to be written
 * @fd: file descriptor
 *
 * Return: the number of bytes read on success, or -1 on error
 */
ssize_t _getline(char **line_ptr, size_t *n, int fd)
{
	ssize_t num_read;
	size_t t_read, buff_size = BUFF_SIZE;

	if (*line_ptr == NULL)
	{
		/* Allocate memory when needed */
		*line_ptr = malloc(sizeof(char) * (buff_size + 1));
		if (*line_ptr == NULL)
			return (-1); /* when memory allocation failed */
	}
	num_read = t_read = 0;
	for (; (num_read = read(fd, *line_ptr + t_read, BUFF_SIZE)) > 0;)
	{
		t_read += num_read;
		/* Resize buffer if needed */
		if (t_read >= buff_size)
		{
			buff_size *= 2;
			*line_ptr = _realloc(*line_ptr, t_read, buff_size);
			if (*line_ptr == NULL)
				return (-1); /* Resizing buffer failed */
			*n = t_read;
		}
		if (t_read && (*line_ptr)[t_read - 1] == '\n')
		{
			(*line_ptr)[t_read] = '\0';
			*n = t_read;
			return (t_read);
		}
	}

	if (num_read == -1)
	{
		free_safely(*line_ptr);
		return (-1); /* Reading from file descriptor failed */
	}
	if (t_read == 0)
		free_safely(*line_ptr); /* No data read, free the memory */

	return (t_read);
}

