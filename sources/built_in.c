#include "minishell.h"

int	pwd(void)
{
	int		error;
    char	path[PATH_MAX + 1];

	error = getcwd(path, PATH_MAX + 1);
	if (!error)
		printf("%s\n", path);
	if (path)
		free(path);
	return (errno);
}

int	cd(const char *path)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;
	char	*temp;

	if (*path == '/')
		chdir(path);
	else
	{
		if (getcwd(path, PATH_MAX + 1))
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