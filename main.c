#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	t_params	*data;
	int			res;

	(void)argc;
	(void)argv;
	data = NULL;
	res = 0;
	if (init(data, env))
		return (EXIT_FAILURE);
	sighandler_set(DEFAULT_MODE);
	cycle(env, data, res);
	return (EXIT_SUCCESS);
}
