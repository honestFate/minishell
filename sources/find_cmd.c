#include "minishell.h"


static int	find_cmd_in_path(char **envp_path, char *cmd, char	**path_to_cmd)
{
	int		i;
	int		err;
	char	*temp;

	i = 0;
	err = 0;
	while (envp_path[i])
	{
		temp = ft_strjoin(envp_path[i], "/");
		*path_to_cmd = ft_strjoin(temp, cmd);
		free(temp);
		access(*path_to_cmd, X_OK);
		if (!errno)
		{
			err = 0;
			break ;
		}
		free(*path_to_cmd);
		if (err == 0 || errno == EACCES)
			err = errno;
		errno = 0;
		++i;
	}
	return (err);
}

/*
set path to executable file in path_to_cmd
return zero if cmd exist, otherwise return error code
and set path_to_cmd to NULL
*/
int	find_cmd(char *cmd, t_env_list *env_list, char **path_to_cmd)
{
	int		i;
	int		err;
	char	**envp_path;

	i = 0;
	while (cmd[i])
	{
		write(1, &cmd[i], 1);
		write(1, "*", 1);
		write(1, "\n", 1);
		++i;
	}
	i = 0;
	*path_to_cmd = NULL;
	printf("cmd - %s, errno - %d\n", cmd, errno);
	access((const char *)cmd, X_OK);
	err = errno;
	printf("cmd - %s, errno - %d\n", cmd, errno);
	errno = 0;
	if (!err)
	{
		*path_to_cmd = ft_strdup(cmd);
		printf("cmd - %s, errno - %d\n", *path_to_cmd, err);
		return (M_OK);
	}
	else if (env_list && cmd[0] != '/' && ft_strncmp(cmd, "./", 2) != 0)
	{
		envp_path = ft_split(ft_getenv(env_list, "PATH"), ':');
		if (!envp_path)
			return (errno);
		err = find_cmd_in_path(envp_path, cmd, path_to_cmd);
		i = 0;
		while (envp_path[i])
			free(envp_path[i++]);
		free(envp_path);
	}
	return (err);
}
