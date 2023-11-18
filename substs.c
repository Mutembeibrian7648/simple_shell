#include "shell.h"

/**
 * is_chain - test if current char in buffer is a chain delimeter
 * ptr_to orgdt: reference to structure of parameters
 * ptr_to bffr: reference to char buffer
 * ptr_to p: reference to & current position in bffr
 * Return: 1 if chain delimeter, 0 otherwise
 */
int is_chain(orgdt_st *orgdt, char *bffr, size_t *p)
{
	size_t j = *p;

	if (bffr[j] == '|' && bffr[j + 1] == '|')
	{
		bffr[j] = 0;
		j++;
		orgdt->cmd_bffr_type = CMD_OR;
	}
	else if (bffr[j] == '&' && bffr[j + 1] == '&')
	{
		bffr[j] = 0;
		j++;
		orgdt->cmd_bffr_type = CMD_AND;
	}
	else if (bffr[j] == ';') /* found end of this command */
	{
		bffr[j] = 0; /* replace semicolon with null */
		orgdt->cmd_bffr_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * check_chain - checks we should continue chaining based on last status
 * ptr_to orgdt: reference to structure of parameters
 * ptr_to bffr: reference to char buffer
 * ptr_to p: reference to & current position in bffr
 * ptr_to i: reference to & starting position in bffr
 * ptr_to len: reference to mem & length of bffr
 * Return: Void
 */
void check_chain(orgdt_st *orgdt, char *bffr, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (orgdt->cmd_bffr_type == CMD_AND)
	{
		if (orgdt->status)
		{
			bffr[i] = 0;
			j = len;
		}
	}
	if (orgdt->cmd_bffr_type == CMD_OR)
	{
		if (!orgdt->status)
		{
			bffr[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * replace_alias - replaces an aliases in the tokenized string
 * ptr_to orgdt: the structure of parameters
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(orgdt_st *orgdt)
{
	int i;
	ordseq *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(orgdt->alias, orgdt->argv[0], '=');
		if (!node)
			return (0);
		free(orgdt->argv[0]);
		p = lctcharstrn(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		orgdt->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - replaces vars in the tokenized string
 * ptr_to orgdt: reference to structure of parameters
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(orgdt_st *orgdt)
{
	int i = 0;
	ordseq *node;

	for (i = 0; orgdt->argv[i]; i++)
	{
		if (orgdt->argv[i][0] != '$' || !orgdt->argv[i][1])
			continue;

		if (!_strcmp(orgdt->argv[i], "$?"))
		{
			replace_string(&(orgdt->argv[i]),
					_strdup(conv_num(orgdt->status, 10, 0)));
			continue;
		}
		if (!_strcmp(orgdt->argv[i], "$$"))
		{
			replace_string(&(orgdt->argv[i]),
					_strdup(conv_num(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(orgdt->env, &orgdt->argv[i][1], '=');
		if (node)
		{
			replace_string(&(orgdt->argv[i]),
					_strdup(lctcharstrn(node->str, '=') + 1));
			continue;
		}
		replace_string(&orgdt->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - replaces string
 * ptr_to old: reference to mem & old string
 * ptr_to new: reference to mem & new string
 * Return: 1 if replaced, otherwise return 0
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
