#include "shell.h"

/**
 * _myenv – outputs the present process environment
 * ptr_to orgdt: Struct of probable args.
 * Return: Always 0
 */
int _myenv(orgdt_st *orgdt)
{
	print_list_str(orgdt->env);
	return (0);
}

/**
 * _getenv – will get the actual value of an environment.
 * ptr_to orgdt: Struct of probable args.
 * ptr_to name: env var name
 * Return: the value
 */
char *_getenv(orgdt_st *orgdt, const char *name)
{
	ordseq *node = orgdt->env;
	char *p;

	while (node)
	{
		p = starts_with(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

int _mysetenv(orgdt_st *orgdt)
{
	if (orgdt->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(orgdt, orgdt->argv[1], orgdt->argv[2]))
		return (0);
	return (1);
}

int _myunsetenv(orgdt_st *orgdt)
{
	int i;

	if (orgdt->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= orgdt->argc; i++)
		_unsetenv(orgdt, orgdt->argv[i]);

	return (0);
}

/**
 * spawn_env_lst – spawns synthetic env linked list
 * ptr_to orgdt: Struct of probable args.
 * Return: Always 0
 */
int spawn_env_lst(orgdt_st *orgdt)
{
	ordseq *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	orgdt->env = node;
	return (0);
}
