#include "shell.h"

/**
 * list_len - determines length of linked list
 * ptr_to h: pointer referencing first node
 * Return: size of list
 */
size_t list_len(const ordseq *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * ordseqo_strings - returns an array of strings of the list->str
 * ptr_to head: pointer referencing first node
 * Return: array of strings
 */
char **ordseqo_strings(ordseq *head)
{
	ordseq *node = head;
	size_t i = list_len(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(_strlen(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = _strcpy(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * print_list - prints all elements of a ordseq linked list
 * ptr_to h: pointer referencing first node
 *
 * Return: size of list
 */
size_t print_list(const ordseq *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(conv_num(h->num, 10, 0));
		_putchar(':');
		_putchar(' ');
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * node_starts_with - returns node whose string starts with prefix
 * ptr_to node: pointer referencing list head
 * ptr_to prefix: string to match
 * ptr_to c: the next character after prefix to match
 *
 * Return: match node or null
 */
ordseq *node_starts_with(ordseq *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = starts_with(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * get_node_index - gets the index of a node
 * ptr_to head: pointer referencing list head
 * ptr_to node: pointer referencing the node
 *
 * Return: index of node or -1
 */
ssize_t get_node_index(ordseq *head, ordseq *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
