#include "minishell.h"

int	env_copy(t_minishell *minishell, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		++i;
	minishell->env_arr = (char **)malloc((i + 1) * sizeof(char *));
	i = 0;
	while (env[i])
	{
		minishell->env_arr[i] = ft_strdup(env[i]);
		if (!minishell->env_arr[i])
		{
			while (--i >= 0)
				free(minishell->env_arr[i]);
			free(minishell->env_arr);
			return (M_ERR);
		}
		++i;
	}
	minishell->env_arr[i] = NULL;
	return (M_ERR);
}

char	*env_create_line(t_env_list *ptr)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(ptr->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, ptr->val);
	free(temp);
	return (result);
}

int	env_fill_array(t_minishell *minishell)
{
	int			i;
	t_env_list	*ptr;

	ptr = minishell->env_list;
	i = 0;
	while (ptr)
	{
		if (ptr->val)
		{
			minishell->env_arr[i] = env_create_line(ptr);
			if (!minishell->env_arr[i])
			{
				while (--i >= 0)
					free(minishell->env_arr[i]);
				return (M_ERR);
			}
			++i;
		}
		ptr = ptr->next;
	}
	minishell->env_arr[i] = NULL;
	ft_putendl_fd("fill array ok", STDOUT_FILENO);
	return (0);
}

int	env_to_array(t_minishell *minishell)
{
	int	i;

	i = 0;
	if (minishell->env_arr)
	{
		while (minishell->env_arr[i])
			free(minishell->env_arr[i++]);
		free(minishell->env_arr[i]);
		free(minishell->env_arr);
	}
	ft_putendl_fd("free arr", STDOUT_FILENO);
	minishell->env_arr = (char **)
			malloc((minishell->env_list_size + 1) * sizeof(char *)); 
	if (!minishell->env_arr)
		return (errno);
	if (!minishell->env_list_size)
	{
		minishell->env_arr[0] = NULL;
		return (M_OK);
	}
	if (env_fill_array(minishell))
	{
		free(minishell->env_arr);
		minishell->env_arr = NULL;
		return (M_ERR);
	}
	return (M_OK);
}

int	env_to_list(t_minishell *minishell, char **envp)
{
	int			i;
	t_env_list	*ptr;

	minishell->env_list = NULL;
	i = 0;
	minishell->env_list_size = i;
	if (!envp[i])
		return (0);
	ptr = new_env_elem(envp[i]);
	if (!ptr)
		return (errno);
	minishell->env_list = ptr;
	++i;
	minishell->env_list_size = i;
	while (envp[i])
	{
		ptr->next = new_env_elem(envp[i]);
		if (!ptr->next)
		{
			env_list_clear(minishell);
			return (errno);
		}
		minishell->env_list_size = i;
		ptr = ptr->next;
		i++;
	}
	ptr->next = NULL;
	return (errno);
}
