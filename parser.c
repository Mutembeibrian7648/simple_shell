#include "shell.h"

/**
 * is_cmd - determines if a file is an executable command
 * ptr_to orgdt: the orgdt struct
 * ptr_to path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(orgdt_st *orgdt, char *path)
{
	struct stat st;

	(void)orgdt;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - duplicates characters
 * ptr_to pathstr: the PATH string
 * ptr_to start: starting index
 * ptr_to stop: stopping index
 * Return: pointer referencing new buffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	static char bffr[1024];
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			bffr[k++] = pathstr[i];
	bffr[k] = 0;
	return (bffr);
}

/**
 * find_path - finds this cmd in the PATH string
 * ptr_to orgdt: the orgdt struct
 * ptr_to pathstr: the PATH string
 * ptr_to cmd: the cmd to find
 * Return: full path of cmd if found else, NULL
 */
char *find_path(orgdt_st *orgdt, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(orgdt, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(orgdt, path))
				return (path);
			if (!pathstr[i])
				break;
			curr_pos = i;
		}
		i++;
	}
	return (NULL);
}
