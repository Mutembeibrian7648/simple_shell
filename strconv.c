#include "shell.h"

/**
 * intermutual - returns true if our shell is working in the interactive mode
 * ptr_to orgdt: reference to the address of a structure
 *
 * Return: 1 if intermutual mode, 0 if contrary
 */
int intermutual(orgdt_st *orgdt)
{
	return (isatty(STDIN_FILENO) && orgdt->readfd <= 2);
}

/**
 * is_delim - checks if character is a delimeter
 * ptr_to c: the char to check
 * ptr_to delim: the delimeter string
 * Return: 1 if true, 0 if false
 */
int is_delim(char c, char *delim)
{
	while (*delim)
		if (*delim++ == c)
			return (1);
	return (0);
}

/**
 * alphabetic_char - checks for alphabetic character
 * ptr_to c: The character to key in.
 * Return: 1 if c is alphabetic, 0 if it is not
 */

int alphabetic_char(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * _strconv – takes a string and converts integer
 * ptr_to s: the string to convert
 * Return: if no numbers return 0 else, converted values
 */

int _strconv(char *s)
{
	int i, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (i = 0; s[i] != '\0' && flag != 2; i++)
	{
		if (s[i] == '-')
			sign *= -1;

		if (s[i] >= '0' && s[i] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (s[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
