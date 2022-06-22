#include "minishell.h"

static t_minishell	*minishell_init(char **envp)
{
	char		*home_path;
	t_minishell	*minishell;

	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (env_copy(minishell, envp) || env_to_list(minishell, envp)
		|| default_env(minishell))
	{
		free_minishell(minishell);
		return(NULL);
	}
	home_path = ft_getenv(minishell->env_list, "HOME");
	if (home_path)
	{
		minishell->history_fd = open_history_file(home_path);
		if (minishell->history_fd < 0)
		{
			free_minishell(minishell);
			return(NULL);
		}
		ft_read_history(minishell->history_fd);
	}
	else
		minishell->history_fd = -1;
	return (minishell);
}

int	init(t_params *data, char *envp)
{
	g_exit_status = 0;
	rl_catch_signals = 0;
	data = malloc(sizeof(t_params));
	if (!data)
		return (M_ERR);
	data->minishell = minishell_init(envp);
	if (!data->minishell)
	{
		free(data);
		return (M_ERR);
	}
	data_init(data);
	return (M_OK);
}
