#include "shell.h"

/**
 * _myhistory - displays the history list,
 * ptr_to orgdt: Struct of probable args.
 *  Return: Always 0
 */
int _myhistory(orgdt_st *orgdt)
{
	print_list(orgdt->history);
	return (0);
}

/**
 * unset_alias – will set an alias to a string
 * ptr_to orgdt: structure of parameters
 * ptr_to str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int unset_alias(orgdt_st *orgdt, char *str)
{
	char *p, c;
	int ret;

	p = lctcharstrn(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = delete_node_at_index(&(orgdt->alias),
		get_node_index(orgdt->alias, node_starts_with(orgdt->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias - sets alias to string
 * ptr_to orgdt: structure of parameters
 * ptr_to str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias(orgdt_st *orgdt, char *str)
{
	char *p;

	p = lctcharstrn(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (unset_alias(orgdt, str));

	unset_alias(orgdt, str);
	return (add_node_end(&(orgdt->alias), str, 0) == NULL);
}

/**
 * print_alias - prints an alias string
 * ptr_to node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int print_alias(ordseq *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = lctcharstrn(node->str, '=');
		for (a = node->str; a <= p; a++)
		_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias – will emulate the pre-built alias
 * ptr_to orgdt: Struct of probable args.
 *  Return: Always 0
 */
int _myalias(orgdt_st *orgdt)
{
	int i = 0;
	char *p = NULL;
	ordseq *node = NULL;

	if (orgdt->argc == 1)
	{
		node = orgdt->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; orgdt->argv[i]; i++)
	{
		p = lctcharstrn(orgdt->argv[i], '=');
		if (p)
			set_alias(orgdt, orgdt->argv[i]);
		else
			print_alias(node_starts_with(orgdt->alias, orgdt->argv[i], '='));
	}

	return (0);
}
