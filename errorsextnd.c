#include "shell.h"

/**
 * _convstr: meant to convert a string to an integer
 * ptr_to s: ref to the string
 * Return: 0 if no numbers in string, else, converted number
 *       -1 on error
 */
int _convstr(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;  /* TODO: why does this make main return 255? */
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error : Prints one or many error messages
 * ptr_to orgdt: address of return orgdt structure
 * ptr_to estr: reference to string containing specified error type
 * Return: 0 if string is empty else, converted number
 *        -1 on error
 */
void print_error(orgdt_st *orgdt, char *estr)
{
	_eputs(orgdt->fname);
	_eputs(": ");
	print_d(orgdt->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(orgdt->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d – meant to print a decimal number
 * ptr_to input: reference to the & input
 * ptr_to fd: reference to & of destination filedescriptor
 * Return: how many characters have been printed
 */
int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * conv_num: function carries out number conversions
 * ptr_to num: reference to the & number
 * ptr_to base: reference to the & base
 * ptr_to flags: reference to the & of flags
 * Return: string
 */
char *conv_num(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * purge_comm - function replaces first instance of '#' with '\0'
 * ptr_to bffr: reference to & string being modified.
 * Return: Always 0;
 */
void purge_comm(char *bffr)
{
	int i;

	for (i = 0; bffr[i] != '\0'; i++)
		if (bffr[i] == '#' && (!i || bffr[i - 1] == ' '))
		{
			bffr[i] = '\0';
			break;
		}
}
