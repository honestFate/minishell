#include "minishell.h"

int	ft_unset(t_minishell *minishell, char **argv)
{
	int	i;
	int	err;

	i = 1;
	err = M_OK;
	while (argv[i])
	{
		if (envvar_validate(argv[i]))
		{
			err = M_ERR;
			print_error(argv[0], INVALID_IDENTIFER, argv[i]);
		}
		else
			envlist_delone(minishell, argv[i]);
		++i;
	}
	return (err);
}
