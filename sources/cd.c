#include "minishell.h"

int	ft_cd(t_minishell *minishell, char **argv)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;

	(void)minishell;
	if (!argv[1])
		return (M_OK);
	if (!getcwd(current_path, PATH_MAX + 1))
		return (M_ERR);
	if (argv[1][0] == '/')
		target_path = ft_strdup(argv[1]);
	else
	{
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, argv[1]);
	}
	if (!target_path)
		return (M_ERR);
	chdir(target_path);
	free(target_path);
	if (!errno)
		envlist_change_val(minishell->env_list, "PWD", target_path);
	else
	{
		print_error(argv[0], errno, argv[1]);
		errno = 0;
		return (M_ERR);
	}
	return (M_OK);
}
