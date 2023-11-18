#include "shell.h"

/**
 **replctstrn – replicates a string into versions of itself
 *ptr_to dest: & recipient string
 *ptr_to src: & of the source string
 *ptr_to n: reference to the number of characters to be copied
 *Return: our concatenated string.
 */
char *replctstrn(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest;

	i = 0;
	while (src[i] != '\0' && i < n - 1)
	{
		dest[i] = src[i];
		i++;
	}
	if (i < n)
	{
		j = i;
		while (j < n)
		{
			dest[j] = '\0';
			j++;
		}
	}
	return (s);
}

/**
 **cnctntstrn: concatenates any two strings
 *ptr_to dest: the first string
 *ptr_to src: the second string
 *ptr_to n: the amount of bytes to be maximally used
 *Return: a concatenated whole of two strings.
 */
char *cnctntstrn(char *dest, char *src, int n)
{
	int i, j;
	char *s = dest;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < n)
	{
		dest[i] = src[j];
		i++;
		j++;
	}
	if (j < n)
		dest[i] = '\0';
	return (s);
}

/**
 **lctcharstrn – locates char components of a string
 *ptr_to s: reference to target
 *ptr_to c: ref to char to look for
 *Return: & a ref of the memory area s
 */
char *lctcharstrn(char *s, char c)
{
	do {
		if (*s == c)
			return (s);
	} while (*s++ != '\0');

	return (NULL);
}
