#include "minishell.h"

int	ft_pwd(t_minishell *minishell, char **argv)
{
    char	path[PATH_MAX + 1];

	(void)minishell;
	(void)argv;
	if (getcwd(path, PATH_MAX + 1) != NULL)
		printf("%s\n", path);
	else
	{
		print_error(argv[0], errno, NULL);
		errno = 0;
		return(M_ERR);
	}
	return (M_OK);
}
