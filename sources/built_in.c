#include "minishell.h"

int	pwd(void)
{
    char	path[PATH_MAX + 1];

	printf("%s\n", getcwd(path, sizeof(path)));
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
		getcwd(current_path, sizeof(current_path));
		if (errno)
			return (errno);
		temp = ft_strjoin(current_path, "/");
		target_path = ft_strjoin(temp, path);
		free(temp);
		printf("%s\n", target_path);
		chdir(target_path); 
		free(target_path);
	}
	return (errno);
}