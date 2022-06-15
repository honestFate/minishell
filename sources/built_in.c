#include "minishell.h"

int	ft_pwd(t_minishell *minishell, char **argv)
{
    char	path[PATH_MAX + 1];

	(void)minishell;
	(void)argv;
	if (getcwd(path, PATH_MAX + 1) != NULL)
		printf("%s\n", path);
	else
		return(M_ERR);
	return (M_OK);
}

int	ft_cd(t_minishell *minishell, char **argv)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;

	(void)minishell;
	if (!argv[1])
		return (0);
	if (argv[2])
		return (TOO_MANY_ARGS);
	if (!getcwd(current_path, PATH_MAX + 1))
		return (M_ERR);
	if (argv[1][0] == '/')
		target_path = ft_strdup(argv[1]);
	else
	{
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, argv[1]);
		if (!target_path)
			return (M_ERR);
		printf("%s\n", target_path); //debug
	}
	chdir(target_path);
	if (!errno)
		envlist_change_val(minishell->env_list, "PWD", target_path);
	if (target_path)
		free(target_path);
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
		printf("%s=%s\n", ptr->key, ptr->val);
		ptr = ptr->next;
	}
	return (0);
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

	i = 1;
	while (argv[i])
	{
		envlist_delone(minishell, argv[i]);
		++i;
	}
	return (M_OK);
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
		printf("exit: %s: numeric argument required\n", argv[0]);
		exit(2);
	}
	while (argv[1][i])
	{
		if (!ft_isdigit(argv[0][i]))
		{
			printf("exit: %s: numeric argument required\n", argv[0]);
			exit(2);
		}
	}
	if (argv[2])
	{
		printf("exit: too many arguments\n");
		return (EXIT_FAILURE);
	}
	exit_status = ft_atoi(argv[1]);
	exit(exit_status);
}

int	ft_export(t_minishell *minishell, char **argv)
{
	t_env_list	*ptr;

	if (argv[1])
	{
		ft_putendl_fd("export put argv to list", STDOUT_FILENO);
		if (envlist_add_var(minishell, argv))
			return (M_ERR);
		return (M_OK);
	}
	for (t_env_list *i = minishell->env_list; i != NULL; i = i->next)
	{
		printf("%s - %s\n", i->key, i->val);
	}
	ft_putendl_fd("shell sort", STDOUT_FILENO);
	select_sort(minishell->env_list);
	ft_putendl_fd("shell sort end", STDOUT_FILENO);
	ptr = minishell->env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "_") != 0)
			printf("declare -x %s=\"%s\"\n", ptr->key, ptr->val);
		ptr = ptr->next;
	}
	ft_putendl_fd("ok", STDOUT_FILENO);
	return (M_OK);
}
