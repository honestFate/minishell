#include "minishell.h"

int	ft_pwd(t_minishell *minishell, char **argv)
{
    char	path[PATH_MAX + 1];

	(void)minishell;
	if (!getcwd(path, PATH_MAX + 1))
		printf("%s\n", path);
	return (errno);
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
		return (errno);
	if (argv[1][0] == '/')
		target_path = ft_strdup(argv[1]);
	else
	{
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, argv[1]);
		if (!target_path)
			return (errno);
		printf("%s\n", target_path); //debug
	}
	chdir(target_path);
	if (!errno)
		env_change_val(minishell->env_list, "PWD", target_path);
	if (target_path)
		free(target_path);
	return (errno);
}

int	ft_env(t_minishell *minishell, char **argv)
{
	t_env_list	*ptr;

	if (argv[1])
		return (USAGE_ERROR);
	ptr = minishell->env_list;
	while (ptr)
	{
		printf("%s=%s\n", ptr->key,
				ptr->val);
		ptr = ptr->next;
	}
	return (0);
}

int	ft_echo(t_minishell *minishell, char **argv)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (strcmp(argv[1], "-n"))
	{
		++flag;
		++i;
	}
	if (argv[i])
	{
		printf("%s", argv[i]);
		++i;
		while (argv[i])
		{
			printf(" %s", argv[i]);
			++i;
		}
	}
	if (!flag)
		printf("\n");
	return (0);
}

int	ft_unset(t_minishell *minishell, char **argv)
{
	int			i;
	int			j;
	t_env_list	*ptr;

	ptr = minishell->env_list;
	j = 0;
	while (ptr)
	{
		i = j;
		if (!argv[i])
			break ;
		while (argv[i])
		{
			if (!strcmp(ptr->key, argv[i]))
			{
				if (ptr == minishell->env_list)
					minishell->env_list = ptr->next;
				ptr = env_del_elem(ptr);
				++j;
				break ;
			}
			++i;
		}
		ptr = ptr->next;
	}
	return (M_OK);
}

int	ft_exit(t_minishell *minishell, char **argv) //need to make better parser 
{
	int				i;
	unsigned char	exit_status;

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
	int	i;
	int	j;

	i = 1;
	if (argv[i])
	{
		while (argv[i])
		{
			if (envlist_add_var(minishell, argv))
				return (errno);
			++i;
		}
		return (M_OK);
	}
	if (env_to_array(minishell))
		return (errno);
	shell_sort(minishell->env_arr, minishell->env_list_size);
	while (minishell->env_arr[i])
	{
		j = 0;
		printf("declare -x ");
		while (minishell->env_arr[i][j] != '=' && minishell->env_arr[i][j])
			printf("%c", minishell->env_arr[i][j++]);
		if (minishell->env_arr[i][j])
		{
			printf("%c\"", minishell->env_arr[i][j++]);
			while (minishell->env_arr[i][j])
				printf("%c", minishell->env_arr[i][j++]);
			printf("\"\n");
		}
		++i;
	}
	return (M_OK);
}
