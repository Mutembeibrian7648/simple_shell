#include "shell.h"

/**
 * _myexit – performs an exit from the simple shell
 * ptr_to orgdt: Struct of probable args.
 * Return: exits in accordance with a given exit status
 * (0) if orgdt.argv[0] != "exit"
 */
int _myexit(orgdt_st *orgdt)
{
	int exitcheck;

	if (orgdt->argv[1]) /* If there is an exit arguement */
	{
		exitcheck = _convstr(orgdt->argv[1]);
		if (exitcheck == -1)
		{
			orgdt->status = 2;
			print_error(orgdt, "Illegal number: ");
			_eputs(orgdt->argv[1]);
			_eputchar('\n');
			return (1);
		}
		orgdt->err_num = _convstr(orgdt->argv[1]);
		return (-2);
	}
	orgdt->err_num = -1;
	return (-2);
}

int _mycd(orgdt_st *orgdt)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!orgdt->argv[1])
	{
		dir = _getenv(orgdt, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = _getenv(orgdt, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(orgdt->argv[1], "-") == 0)
	{
		if (!_getenv(orgdt, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(_getenv(orgdt, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = _getenv(orgdt, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(orgdt->argv[1]);
	if (chdir_ret == -1)
	{
		print_error(orgdt, "can't cd to ");
		_eputs(orgdt->argv[1]), _eputchar('\n');
	}
	else
	{
		_setenv(orgdt, "OLDPWD", _getenv(orgdt, "PWD="));
		_setenv(orgdt, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * _myhelp – will change the directory of the process
 * ptr_to orgdt: Struct of probable args.
 * Return: Always 0
 */
int _myhelp(orgdt_st *orgdt)
{
	char **arg_array;

	arg_array = orgdt->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
