#include "shell.h"

/**
 * hsh - main shell loop
 * ptr_to orgdt: the parameter & return orgdt struct
 * ptr_to av: the argument vector from main()
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(orgdt_st *orgdt, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_orgdt(orgdt);
		if (intermutual(orgdt))
			_puts("$ ");
		_eputchar(BFFR_FLUSH);
		r = get_feed(orgdt);
		if (r != -1)
		{
			set_orgdt(orgdt, av);
			builtin_ret = find_builtin(orgdt);
			if (builtin_ret == -1)
				find_cmd(orgdt);
		}
		else if (intermutual(orgdt))
			_putchar('\n');
		free_orgdt(orgdt, 0);
	}
	write_history(orgdt);
	free_orgdt(orgdt, 1);
	if (!intermutual(orgdt) && orgdt->status)
		exit(orgdt->status);
	if (builtin_ret == -2)
	{
		if (orgdt->err_num == -1)
			exit(orgdt->status);
		exit(orgdt->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * ptr_to orgdt: the parameter & return orgdt struct
 * Return: -1 if builtin not found,
 * 0 if builtin executed successfully,
 * 1 if builtin found but not successful,
 * 2 if builtin signals exit()
 */
int find_builtin(orgdt_st *orgdt)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(orgdt->argv[0], builtintbl[i].type) == 0)
		{
			orgdt->line_count++;
			built_in_ret = builtintbl[i].func(orgdt);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * ptr_to orgdt: the parameter & return orgdt struct
 * Return: void
 */
void find_cmd(orgdt_st *orgdt)
{
	char *path = NULL;
	int i, k;

	orgdt->path = orgdt->argv[0];
	if (orgdt->linecount_flag == 1)
	{
		orgdt->line_count++;
		orgdt->linecount_flag = 0;
	}
	for (i = 0, k = 0; orgdt->arg[i]; i++)
		if (!is_delim(orgdt->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(orgdt, _getenv(orgdt, "PATH="), orgdt->argv[0]);
	if (path)
	{
		orgdt->path = path;
		fork_cmd(orgdt);
	}
	else
	{
		if ((intermutual(orgdt) || _getenv(orgdt, "PATH=")
					|| orgdt->argv[0][0] == '/') && is_cmd(orgdt, orgdt->argv[0]))
			fork_cmd(orgdt);
		else if (*(orgdt->arg) != '\n')
		{
			orgdt->status = 127;
			print_error(orgdt, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * ptr_to orgdt: the parameter & return orgdt struct
 * Return: void
 */
void fork_cmd(orgdt_st *orgdt)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(orgdt->path, orgdt->argv, get_environ(orgdt)) == -1)
		{
			free_orgdt(orgdt, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(orgdt->status));
		if (WIFEXITED(orgdt->status))
		{
			orgdt->status = WEXITSTATUS(orgdt->status);
			if (orgdt->status == 126)
				print_error(orgdt, "Permission denied\n");
		}
	}
}
