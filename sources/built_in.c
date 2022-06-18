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

int	ft_env(t_minishell *minishell, char **argv)
{
	t_env_list	*ptr;

	if (argv[1])
		return (USAGE_ERROR);
	ptr = minishell->env_list;
	while (ptr)
	{
		if (ptr->val)
		{
			ft_putstr_fd(ptr->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(ptr->val, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		ptr = ptr->next;
	}
	return (M_OK);
}

int	ft_echo(t_minishell *minishell, char **argv)
{
	int	i;
	int	flag;

	(void)minishell;
	i = 1;
	flag = 0;
	while (argv[i] && !strcmp(argv[i], "-n"))
	{
		++flag;
		++i;
	}
	if (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		++i;
		while (argv[i])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(argv[i], STDOUT_FILENO);
			++i;
		}
	}
	if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (M_OK);
}



int	ft_unset(t_minishell *minishell, char **argv)
{
	int	i;
	int	err;

	i = 1;
	err = M_OK;
	while (argv[i])
	{
		if (envvar_validate(argv[i]))
		{
			err = M_ERR;
			print_error(argv[0], EINVAL, argv[i]);
		}
		else
			envlist_delone(minishell, argv[i]);
		++i;
	}
	return (err);
}

int	ft_exit(t_minishell *minishell, char **argv) //need to make better parser 
{
	int				i;
	unsigned char	exit_status;

	(void)minishell;
	i = 0;
	if (!argv[1])
		exit(EXIT_SUCCESS);
	while (ft_isspace(argv[1][i]))
		++i;
	if (argv[1][i])
		++i;
	if (check_overflow(argv[1]) == M_ERR)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		print_error(argv[0], EINVAL, argv[1]);
		exit(255);
	}
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[1][i]))
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			print_error(argv[0], E2BIG, NULL);
			exit(255);
		}
	}
	if (argv[2])
	{
		print_error(argv[0], E2BIG, NULL);
		return (M_ERR);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
}

int	ft_export(t_minishell *minishell, char **argv)
{
	int			i;
	int			err;
	t_env_list	*ptr;

	i = 1;
	err = 0;
	if (argv[i])
	{
		while (argv[i])
		{
			if (envvar_validate(argv[i]))
			{
				err = M_ERR;
				print_error(argv[0], EINVAL, argv[i]);
			}
			if (envlist_add_var(minishell, argv))
				return (M_ERR);
			++i;
		}
		return (err);
	}
	for (t_env_list *i = minishell->env_list; i != NULL; i = i->next)
	{
		printf("%s - %s\n", i->key, i->val);
	}
	select_sort(minishell->env_list);
	ptr = minishell->env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "_") != 0)
			printf("declare -x %s=\"%s\"\n", ptr->key, ptr->val);
		ptr = ptr->next;
	}
	return (M_OK);
}
