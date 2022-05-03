#include "minishell.h"

int	env_copy(char ***dst, char **env)
{
	int	i;

	i = 0;
	while (env[i])
		++i;
	*dst = malloc((i + 1) * sizeof(char **));
	i = 0;
	while (env[i])
	{
		*dst[i] = ft_strdup(env[i]);
		if (!*dst[i])
		{
			while (--i >= 0)
				free(*dst[i]);
			free(*dst);
			return (errno);
		}
		++i;
	}
	*dst[i] = NULL;
	return (errno);
}

void	env_fill_line(char *line, env_list_t *ptr, int dst_size)
{
	ft_strlcat(line, ptr->key, dst_size);
	ft_strlcat(line, "=", dst_size);
	if (ptr->val)
		ft_strlcat(line, ptr->val, dst_size);
}

int	env_fill_array(minishell_t *minishell)
{
	int			i;
	int			dst_size;
	env_list_t	*ptr;

	ptr = minishell->env_list;
	i = 0;
	while (ptr)
	{
		dst_size = ft_strlen(ptr->key);
		if (ptr->val)
			dst_size += ft_strlen(ptr->val);
		++dst_size;
		minishell->env_arr[i] = (char *)malloc(dst_size * sizeof(char));
		if (!minishell->env_arr[i])
		{
			while (--i >= 0)
				free(minishell->env_arr[i]);
			return (errno);
		}
		env_fill_line(minishell->env_arr[i], ptr, dst_size);
		ptr = ptr->next;
		++i;
	}
	minishell->env_arr[i] = NULL;
	return (0);
}

int	env_to_array(minishell_t *minishell)
{
	if (minishell->env_arr)
		free(minishell->env_arr);
	minishell->env_arr = (char **)
			malloc((minishell->env_list_size + 1) * sizeof(char *)); 
	if (!minishell->env_arr)
		return (errno);
	if (!minishell->env_list_size)
	{
		minishell->env_arr[0] = NULL;
		return (0);
	}
	if (env_fill_array(minishell))
	{
		free(minishell->env_arr);
		minishell->env_arr = NULL;
		return (errno);
	}
	return (0);
}

int	env_to_list(minishell_t *minishell, char **envp)
{
	int			i;
	env_list_t	*ptr;

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
