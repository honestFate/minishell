#include "minishell.h"

int	pwd(void)
{
    char	path[PATH_MAX + 1];

	if (!getcwd(path, PATH_MAX + 1))
		printf("%s\n", path);
	return (errno);
}

int	cd(char *path)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;

	if (*path == '/')
		chdir(path);
	else
	{
		if (!getcwd(current_path, PATH_MAX + 1))
			return (errno);
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, path);
		if (!target_path)
			return (errno);
		printf("%s\n", target_path); //debug
		chdir(target_path);
		free(target_path);
	}
	return (errno);
}

int	env(char **env_arr)
{
	int	i;

	i = 0;
	while (env_arr[i])
	{
		printf("%s\n", env_arr[i]);
		i++;
	}
	return (0);
}

int	echo(char **argv, int flag)
{
	int	i;

	while (argv[i])
	{
		printf("%s ", argv[i]);

	}
	
	return (0);
}