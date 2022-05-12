#include "minishell.h"

int	find_cmd(char *cmd, env_list_t *env_list, char *path_to_cmd)
{
	int		i;
	char	*path;
	int		err;
	char	*envp_path;

	path_to_cmd = NULL;
	access(cmd, X_OK);
	err = errno;
	errno = 0;
	if (!err)
		path_to_cmd = strdup(cmd);
	else if (env_list)
	{
		path = ft_getenv(env_list, "PATH");
		if (path)
		{
			envp_path = ft_split(path, ":");
			if (envp_path)
			{
				while (envp_path[i])
				{
					access(envp_path[i], X_OK);
					if (!errno)
					{
						err = 0;
						path_to_cmd = strdup(cmd);
						break ;
					}
					if (err != EACCES)
						err = errno;
					errno = 0;
					++i;
				}
				i = 0;
				while (envp_path[i])
					free(envp_path[i++]);
				free(envp_path);
			}
			free(path);
		}
	}
	return (err);
}
