#include "shell.h"

/**
 * _eputs - prints an input string
 * ptr_to str: reference to the string intended for print
 * Return: Nothing
 */
void _eputs(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		_eputchar(str[i]);
		i++;
	}
}

/**
 * _eputchar – writes character c to stderr
 * ptr_to c: reference to the character to print
 * Return: On success 1.
 * On error, returns -1, and the appropriate errno is set.
 */
int _eputchar(char c)
{
	static int i;
	static char bffr[WRITE_BFFR_SIZE];

	if (c == BFFR_FLUSH || i >= WRITE_BFFR_SIZE)
	{
		write(2, bffr, i);
		i = 0;
	}
	if (c != BFFR_FLUSH)
		bffr[i++] = c;
	return (1);
}

/**
 * _putfd – intended to write character c to the appropriate file descriptor.
 * ptr_to c: The character that should be printed
 * ptr_to fd: The fd to write our character to
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _putfd(char c, int fd)
{
	static int i;
	static char bffr[WRITE_BFFR_SIZE];

	if (c == BFFR_FLUSH || i >= WRITE_BFFR_SIZE)
	{
		write(fd, bffr, i);
		i = 0;
	}
	if (c != BFFR_FLUSH)
		bffr[i++] = c;
	return (1);
}

/**
 * _putsfd – is intended to print an input string.
 * ptr_to str: reference to the string
 * ptr_to fd: reference to the fd to print to
 * Return: the number of chars put
 */
int _putsfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _putfd(*str++, fd);
	}
	return (i);
}
