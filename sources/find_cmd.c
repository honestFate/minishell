#include "minishell.h"

int	find_cmd(char *cmd, t_env_list *env_list, char *path_to_cmd)
{
	int		i;
	char	*path;
	int		err;
	char	*temp;
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
		if (!path)
			return (err);
		envp_path = ft_split(path, ":");
		free(path);
		if (!envp_path)
			return (err);
		while (envp_path[i])
		{
			temp = ft_strjoin(envp_path[i], "/");
			path_to_cmd = ft_strjoin(temp, cmd);
			free(temp);
			access(path_to_cmd, X_OK);
			if (!errno)
			{
				err = 0;
				break ;
			}
			free(path_to_cmd);
			if (errno == EACCES)
				err = errno;
			errno = 0;
			++i;
		}
		i = 0;
		while (envp_path[i])
			free(envp_path[i++]);
		free(envp_path);
	}
	return (err);
}
