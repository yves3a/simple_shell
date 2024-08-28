#include "shell.h"

/**
 * _itoa - Converts an integer to a string.
 * @n: The integer to convert.
 * @str: The buffer to store the converted string.
 *
 * Description: This function takes an integer @n and converts it to a
 * string representation, storing it in the provided buffer @str.
 */
void _itoa(size_t n, char *str)
{
	int index = 0;

	do {
		str[index++] = n % 10 + '0'; /* Get next digit */
	} while ((n /= 10) > 0);

	str[index] = '\0';

	_reverse(str, index);
}

/**
 * _reverse - Reverses a string in place.
 * @buff: The string to reverse.
 * @len: The length of the string.
 *
 * Description: This function takes a string @buffer of length @len and
 * reverses its contents in place.
 */
void _reverse(char *buff, size_t len)
{
	size_t index, m;
	char c;

	if (!buff)
		return; /* Handle invalid string */

	m = len / 2;

	for (index = 0; index < m; index++)
	{
		c = buff[index];

		/* Swap values */
		buff[index] = buff[len - index - 1];
		buff[len - index - 1] = c;
	}
}

/**
 * _atoi - Converts a string to an integer.
 * @str: The string to convert.
 *
 * Return: The converted integer value, or 0 if the string is invalid.
 *
 * Description: This function takes a string @s and converts it to its
 * integer representation. If the string does not contain a valid
 * integer, the function returns 0.
 */
int _atoi(const char *str)
{
	unsigned int n = 0;
	int index, sign;

	sign = 1;

	for (index = 0; !(isdigit(str[index])); index++)
	{
		if (str[index] == '\0') /* Handle empty string or no digits */
			break;

		else if (isspace(str[index]) || isalpha(str[index]))
			continue; /* Skip characters and whitespaces */

		else if (issign(str[index]))
			sign *= isnegative(str[index]); /* Update sign */
	}

	/* Convert the string to a number */
	while (isdigit(str[index]))
	{
		n = 10 * n + (str[index] - '0');
		index++;
	}

	return (n * sign);
}

