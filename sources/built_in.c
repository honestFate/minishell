#include "minishell.h"

int	ft_pwd(void)
{
    char	path[PATH_MAX + 1];

	if (!getcwd(path, PATH_MAX + 1))
		printf("%s\n", path);
	return (errno);
}

int	ft_cd(char *path)
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

int	ft_env(env_list_t *env_list)
{
	while (env_list)
	{
		printf("%s=%s\n", env_list->key, env_list->val);
		env_list = env_list->next;
	}
	return (0);
}

int	ft_echo(char **argv, int flag)
{
	int	i;

	i = 0;
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

int	ft_unset(minishell_t *minishell, char **argv)
{
	int			i;
	env_list_t	*ptr;

	ptr = minishell->env_list;
	while (ptr)
	{
		i = 0;
		while (argv[i])
		{
			if (!strcmp(ptr->key, argv[i]))
			{
				if (ptr == minishell->env_list)
					minishell->env_list = ptr->next;
				ptr = env_del_elem(ptr);
				break ;
			}
			++i;
		}
		ptr = ptr->next;
	}
	return (0);
}

int	ft_exit(char **argv) //need to make better parser 
{
	int32_t	i;

	i = 0;
	while (argv[0][i])
	{
		if (!isdigit(argv[0][i]))
		{
			printf("exit: %s: numeric argument required\n", argv[0]);
			exit(255);
		}
	}
	if (argv[0])
	{
		if (argv[1])
		{
			printf("exit: too many arguments\n");
			return (1);
		}
		exit(ft_atoi(argv[0]));
	}
	exit(0);
}


int	ft_export(minishell_t *minishell, char **argv)
{
	int	i;
	int	j;

	if (argv[0])
		return (envlist_add_var(minishell, argv));
	i = 0;
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
	return (0);
}
