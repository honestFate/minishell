#include "minishell.h"

char	*find_cmd(char *cmd, t_env_list *env_list)
{
	int		i;
	char	*path;
	int		err;
	char	*temp;
	char	**envp_path;
	char	*path_to_cmd;
	char	pwd[PATH_MAX + 1];

	path_to_cmd = NULL;
	access(cmd, X_OK);
	err = errno;
	errno = 0;
	if (!err)
		return (ft_strdup(cmd));
	if (ft_strncmp(cmd, "./", 2) == 0)
	{
		write(1, "./ govno\n", 10);
		if (getcwd(pwd, PATH_MAX + 1) == NULL)
			return (NULL);
		ft_putendl_fd(cmd, 1);
		if (ft_strncmp(cmd, "./", 2) == 0)
		{
			printf("ERRNO - %d, EBLAN?\n", errno);
			write(1, "./ govno\n", 10);
			char *tmp = ft_strdup(cmd + 1);
			char *relative_path = ft_strjoin(pwd, tmp);
			free(tmp);
			ft_putendl_fd(relative_path, 1);
			printf("strlen - %d\n", ft_strlen(relative_path));
			printf("ERRNO - %d, EBLAN?\n", errno);
			access(relative_path, X_OK);
			if (!errno)
			{
				err = 0;
				return (relative_path);
			}
			printf("ERRNO - %d, EBLAN?\n", errno);
			free(relative_path);
			if (errno == EACCES)
				err = errno;
		}
		else
		{
			
			path_to_cmd = ft_strdup(cmd);
			return (path_to_cmd);
		}
	}
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
