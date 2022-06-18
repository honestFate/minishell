#include "minishell.h"

int	ft_exit(t_minishell *minishell, t_pipe_line *pipe_line) //need to make better parser 
{
	int				i;
	unsigned char	exit_status;

	(void)minishell;
	i = 0;
	if (!pipe_line->argv[1])
		exit(EXIT_SUCCESS);
	while (ft_isspace(pipe_line->argv[1][i]))
		++i;
	if (pipe_line->argv[1][i])
		++i;
	if (check_overflow(pipe_line->argv[1]) == M_ERR)
		exit_minishell(minishell, pipe_line, 255, pipe_line->argv[1]);
	while (pipe_line->argv[1][i])
	{
		if (!ft_isdigit(pipe_line->argv[1][i]))
			exit_minishell(minishell, pipe_line, 255, pipe_line->argv[1]);
	}
	if (pipe_line->argv[2])
	{
		print_error(pipe_line->argv[0], E2BIG, NULL);
		return (M_ERR);
	}
	exit_status = ft_atoi(pipe_line->argv[1]);
	exit_minishell(minishell, pipe_line, EXIT_SUCCESS, NULL);
}
