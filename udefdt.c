#include "shell.h"

ordseq *add_node(ordseq **head, const char *str, int num)
{
	ordseq *new_head;

	if (!head)
		return (NULL);
	new_head = malloc(sizeof(ordseq));
	if (!new_head)
		return (NULL);
	_memset((void *)new_head, 0, sizeof(ordseq));
	new_head->num = num;
	if (str)
	{
		new_head->str = _strdup(str);
		if (!new_head->str)
		{
			free(new_head);
			return (NULL);
		}
	}
	new_head->next = *head;
	*head = new_head;
	return (new_head);
}

/**
 * add_node_end - adds a node to the end of the list
 * ptr_to head: address of pointer referencing head node
 * ptr_to str: str field of node
 * ptr_to num: node index used by history
 * Return: size of list
 */
ordseq *add_node_end(ordseq **head, const char *str, int num)
{
	ordseq *new_node, *node;

	if (!head)
		return (NULL);

	node = *head;
	new_node = malloc(sizeof(ordseq));
	if (!new_node)
		return (NULL);
	_memset((void *)new_node, 0, sizeof(ordseq));
	new_node->num = num;
	if (str)
	{
		new_node->str = _strdup(str);
		if (!new_node->str)
		{
			free(new_node);
			return (NULL);
		}
	}
	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = new_node;
	}
	else
		*head = new_node;
	return (new_node);
}

/**
 * print_list_str: prints str element of a ordseq linked list
 * ptr_to h: & first node
 * Return: the actual size of list
 */
size_t print_list_str(const ordseq *h)
{
	size_t i = 0;

	while (h)
	{
		_puts(h->str ? h->str : "(nil)");
		_puts("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * delete_node_at_index - deletes node at given index
 * ptr_to head: address of pointer referencing first node
 * ptr_to index: index of node to delete
 * Return: 1 on success, 0 on failure
 */
int delete_node_at_index(ordseq **head, unsigned int index)
{
	ordseq *node, *prev_node;
	unsigned int i = 0;

	if (!head || !*head)
		return (0);

	if (!index)
	{
		node = *head;
		*head = (*head)->next;
		free(node->str);
		free(node);
		return (1);
	}
	node = *head;
	while (node)
	{
		if (i == index)
		{
			prev_node->next = node->next;
			free(node->str);
			free(node);
			return (1);
		}
		i++;
		prev_node = node;
		node = node->next;
	}
	return (0);
}

/**
 * free_list - frees every node in a list
 * ptr_to head_ptr: & pointer referencing head node
 * Return: void
 */
void free_list(ordseq **head_ptr)
{
	ordseq *node, *next_node, *head;

	if (!head_ptr || !*head_ptr)
		return;
	head = *head_ptr;
	node = head;
	while (node)
	{
		next_node = node->next;
		free(node->str);
		free(node);
		node = next_node;
	}
	*head_ptr = NULL;
}
