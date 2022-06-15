#include "minishell.h"

char	*find_cmd(char *cmd, t_env_list *env_list)
{
	int		i;
	char	*path;
	int		err;
	char	*temp;
	char	**envp_path;
	char	*path_to_cmd;

	path_to_cmd = NULL;
	access(cmd, X_OK);
	err = errno;
	errno = 0;
	if (!err)
		path_to_cmd = ft_strdup(cmd);
	else if (env_list)
	{
		path = ft_getenv(env_list, "PATH");
		if (!path)
			return (NULL);
		envp_path = ft_split(path, ':');
		free(path);
		if (!envp_path)
			return (NULL);
		i = 0;
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
	return (path_to_cmd);
}
