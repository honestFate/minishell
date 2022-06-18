#include "minishell.h"

int	ft_exit(t_minishell *minishell, char **argv) //need to make better parser 
{
	int				i;
	unsigned char	exit_status;

	(void)minishell;
	i = 0;
	if (!argv[1])
		exit(EXIT_SUCCESS);
	while (ft_isspace(argv[1][i]))
		++i;
	if (argv[1][i])
		++i;
	if (check_overflow(argv[1]) == M_ERR)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		print_error(argv[0], NOT_NUM_ARG, argv[1]);
		exit(255);
	}
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			print_error(argv[0], NOT_NUM_ARG, NULL);
			exit(255);
		}
	}
	if (argv[2])
	{
		print_error(argv[0], E2BIG, NULL);
		return (M_ERR);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
}
