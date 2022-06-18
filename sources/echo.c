#include "minishell.h"

int	ft_echo(t_minishell *minishell, char **argv)
{
	int	i;
	int	flag;

	(void)minishell;
	i = 1;
	flag = 0;
	while (argv[i] && !strcmp(argv[i], "-n"))
	{
		++flag;
		++i;
	}
	if (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		++i;
		while (argv[i])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(argv[i], STDOUT_FILENO);
			++i;
		}
	}
	if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (M_OK);
}
