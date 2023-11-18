#include "shell.h"

/**
 * input_bffr – creates a buffer for chained cmds
 * ptr_to orgdt: structure of parameters
 * ptr_to bffr: & buffer
 * ptr_to len: & len var
 * Return: number of bytes read
 */
ssize_t input_bffr(orgdt_st *orgdt, char **bffr, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*freebfrstrm((void **)orgdt->cmd_bffr);*/
		free(*bffr);
		*bffr = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(bffr, &len_p, stdin);
#else
		r = _getline(orgdt, bffr, &len_p);
#endif
		if (r > 0)
		{
			if ((*bffr)[r - 1] == '\n')
			{
				(*bffr)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			orgdt->linecount_flag = 1;
			purge_comm(*bffr);
			build_history_list(orgdt, *bffr, orgdt->histcount++);
			/* if (lctcharstrn(*bffr, ';')) is this a command chain? */
			{
				*len = r;
				orgdt->cmd_bffr = bffr;
			}
		}
	}
	return (r);
}

/**
 * get_feed - gets a line
 * ptr_to orgdt: structure of parameters
 * Return: amount of bytes read
 */
ssize_t get_feed(orgdt_st *orgdt)
{
	static char *bffr; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **bffr_p = &(orgdt->arg), *p;

	_putchar(BFFR_FLUSH);
	r = input_bffr(orgdt, &bffr, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len) /* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current bffr position */
		p = bffr + i; /* get pointer for return */

		check_chain(orgdt, bffr, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (is_chain(orgdt, bffr, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			orgdt->cmd_bffr_type = CMD_NORM;
		}

		*bffr_p = p; /* pass back pointer referencing current command position */
		return (_strlen(p)); /* return length of current command */
	}

	*bffr_p = bffr; /* else not a chain, pass back buffer from _getline() */
	return (r); /* return length of buffer from _getline() */
}

/**
 * read_bffr - reads a buffer
 * ptr_to orgdt: structure of parameters
 * ptr_to bffr: buffer
 * ptr_to i: size
 *
 * Return: r
 */
ssize_t read_bffr(orgdt_st *orgdt, char *bffr, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(orgdt->readfd, bffr, READ_BFFR_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - gets the next line of input from STDIN
 * ptr_to orgdt: structure of parameters
 * ptr_to ptr: address of pointer referencing buffer, preallocated or NULL
 * ptr_to length: size of preallocated ptr buffer if not NULL
 * Return: s
 */
int _getline(orgdt_st *orgdt, char **ptr, size_t *length)
{
	static char bffr[READ_BFFR_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_bffr(orgdt, bffr, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = lctcharstrn(bffr + i, '\n');
	k = c ? 1 + (unsigned int)(c - bffr) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		cnctntstrn(new_p, bffr + i, k - i);
	else
		replctstrn(new_p, bffr + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - blocks ctrl-C
 * ptr_to sig_num: the signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BFFR_FLUSH);
}
