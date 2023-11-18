#include "shell.h"

/**
 * get_history_file - gets the history file
 * ptr_to orgdt: structure of parameters
 * Return: allocated string containg history file
 */

char *get_history_file(orgdt_st *orgdt)
{
	char *bffr, *dir;

	dir = _getenv(orgdt, "HOME=");
	if (!dir)
		return (NULL);
	bffr = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!bffr)
		return (NULL);
	bffr[0] = 0;
	_strcpy(bffr, dir);
	_strcat(bffr, "/");
	_strcat(bffr, HIST_FILE);
	return (bffr);
}

/**
 * write_history - creates a file, or appends to an existing file
 * ptr_to orgdt: the structure of parameters
 * Return: 1 on success, else -1
 */
int write_history(orgdt_st *orgdt)
{
	ssize_t fd;
	char *filename = get_history_file(orgdt);
	ordseq *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = orgdt->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BFFR_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history – will read history from any file
 * ptr_to orgdt: the structure of parameters
 * Return: histcount on success, 0 otherwise
 */
int read_history(orgdt_st *orgdt)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *bffr = NULL, *filename = get_history_file(orgdt);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	bffr = malloc(sizeof(char) * (fsize + 1));
	if (!bffr)
		return (0);
	rdlen = read(fd, bffr, fsize);
	bffr[fsize] = 0;
	if (rdlen <= 0)
		return (free(bffr), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (bffr[i] == '\n')
		{
			bffr[i] = 0;
			build_history_list(orgdt, bffr + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(orgdt, bffr + last, linecount++);
	free(bffr);
	orgdt->histcount = linecount;
	while (orgdt->histcount-- >= HIST_MAX)
		delete_node_at_index(&(orgdt->history), 0);
	renumber_history(orgdt);
	return (orgdt->histcount);
}

/**
 * build_history_list - adds entry to a history linked list
 * ptr_to orgdt: Struct of probable args. Used to maintain
 * ptr_to bffr: buffer
 * ptr_to linecount: the history linecount, histcount
 * Return: Always 0
 */
int build_history_list(orgdt_st *orgdt, char *bffr, int linecount)
{
	ordseq *node = NULL;

	if (orgdt->history)
		node = orgdt->history;
	add_node_end(&node, bffr, linecount);

	if (!orgdt->history)
		orgdt->history = node;
	return (0);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * ptr_to orgdt: Struct of probable args. Used to maintain
 * Return: the new histcount
 */
int renumber_history(orgdt_st *orgdt)
{
	ordseq *node = orgdt->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (orgdt->histcount = i);
}
