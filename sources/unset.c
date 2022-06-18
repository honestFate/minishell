#include "minishell.h"

int	ft_unset(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	i;
	int	err;

	i = 1;
	err = M_OK;
	while (pipe_line->argv[i])
	{
		if (envvar_validate(pipe_line->argv[i]))
		{
			err = M_ERR;
			print_error(pipe_line->argv[0], INVALID_IDENTIFER, pipe_line->argv[i]);
		}
		else
			envlist_delone(minishell, pipe_line->argv[i]);
		++i;
	}
	return (err);
}
