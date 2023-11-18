#include "shell.h"

/**
 * freebfrstrm - frees a pointer and NULLs the address
 * ptr_to ptr: address of the pointer referencing free
 * Return: 1 if freed, otherwise 0.
 */
int freebfrstrm(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
