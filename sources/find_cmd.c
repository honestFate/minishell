#include "minishell.h"

int	find_cmd(char *cmd, env_list_t *env_list, char *path_to_cmd)
{
	int		i;
	char	*path;
	char	*err;
	char	*envp_path;

	path_to_cmd = NULL;
	access(cmd, X_OK);
	if (!errno)
		path_to_cmd = strdup(cmd);
	else if (env_list && errno != EACCES)
	{
		path = ft_getenv(env_list, "PATH");
		if (path)
		{
			envp_path = ft_split(path, ":");
			if (envp_path)
			{
				i = 0;
				while (envp_path[++i]);
				--i;
				while (envp_path[i])
				{
					access(envp_path[i], X_OK);

					--i;
				}
				
			}
		}
	}
	return (errno);
}
