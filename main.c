#include "shell.h"

/**
 * main - entry point
 * ptr_to ac: arg count
 * ptr_to av: arg vector
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	orgdt_st orgdt[] = { ORGDT_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (fd)
			: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs(": 0: Can't open ");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(BFFR_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		orgdt->readfd = fd;
	}
	spawn_env_lst(orgdt);
	read_history(orgdt);
	hsh(orgdt, av);
	return (EXIT_SUCCESS);
}
