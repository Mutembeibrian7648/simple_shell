#include "shell.h"

/**
 * clear_orgdt - initializes orgdt_st struct
 * ptr_to orgdt: reference to the address of a structure
 */
void clear_orgdt(orgdt_st *orgdt)
{
	orgdt->arg = NULL;
	orgdt->argv = NULL;
	orgdt->path = NULL;
	orgdt->argc = 0;
}

/**
 * set_orgdt - initializes orgdt_st struct
 * ptr_to orgdt: reference to the address of a structure
 * ptr_to av: argument vector
 */
void set_orgdt(orgdt_st *orgdt, char **av)
{
	int i = 0;

	orgdt->fname = av[0];
	if (orgdt->arg)
	{
		orgdt->argv = strtow(orgdt->arg, " \t");
		if (!orgdt->argv)
		{
			orgdt->argv = malloc(sizeof(char *) * 2);
			if (orgdt->argv)
			{
				orgdt->argv[0] = _strdup(orgdt->arg);
				orgdt->argv[1] = NULL;
			}
		}
		for (i = 0; orgdt->argv && orgdt->argv[i]; i++)
			;
		orgdt->argc = i;

		replace_alias(orgdt);
		replace_vars(orgdt);
	}
}

/**
 * free_orgdt - frees orgdt_st struct fields
 * ptr_to orgdt: reference to the address of a structure
 * ptr_to all: true if freeing all fields
 */
void free_orgdt(orgdt_st *orgdt, int all)
{
	ffree(orgdt->argv);
	orgdt->argv = NULL;
	orgdt->path = NULL;
	if (all)
	{
		if (!orgdt->cmd_bffr)
			free(orgdt->arg);
		if (orgdt->env)
			free_list(&(orgdt->env));
		if (orgdt->history)
			free_list(&(orgdt->history));
		if (orgdt->alias)
			free_list(&(orgdt->alias));
		ffree(orgdt->environ);
			orgdt->environ = NULL;
		freebfrstrm((void **)orgdt->cmd_bffr);
		if (orgdt->readfd > 2)
			close(orgdt->readfd);
		_putchar(BFFR_FLUSH);
	}
}
