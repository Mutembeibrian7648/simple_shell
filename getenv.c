#include "shell.h"

/**
 * get_environ – outputs string array copy of our environment
 * ptr_to orgdt: Struct of probable args.
 * Return: Always 0
 */
char **get_environ(orgdt_st *orgdt)
{
	if (!orgdt->environ || orgdt->env_changed)
	{
		orgdt->environ = ordseqo_strings(orgdt->env);
		orgdt->env_changed = 0;
	}

	return (orgdt->environ);
}

/**
 * _unsetenv - Remove an environment variable
 * ptr_to orgdt: Struct of probable args.
 *  Return: 1 on delete, 0 otherwise
 * ptr_to var: the string env var property
 */
int _unsetenv(orgdt_st *orgdt, char *var)
{
	ordseq *node = orgdt->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			orgdt->env_changed = delete_node_at_index(&(orgdt->env), i);
			i = 0;
			node = orgdt->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (orgdt->env_changed);
}

/**
 * _setenv - Initialize a new environment variable,
 *             or modify an existing one
 * ptr_to orgdt: Struct of probable args. Used to maintain
 *        constant function prototype.
 * ptr_to var: the string env var property
 * ptr_to value: the string env var value
 *  Return: Always 0
 */
int _setenv(orgdt_st *orgdt, char *var, char *value)
{
	char *bffr = NULL;
	ordseq *node;
	char *p;

	if (!var || !value)
		return (0);

	bffr = malloc(_strlen(var) + _strlen(value) + 2);
	if (!bffr)
		return (1);
	_strcpy(bffr, var);
	_strcat(bffr, "=");
	_strcat(bffr, value);
	node = orgdt->env;
	while (node)
	{
		p = starts_with(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = bffr;
			orgdt->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(orgdt->env), bffr, 0);
	free(bffr);
	orgdt->env_changed = 1;
	return (0);
}
