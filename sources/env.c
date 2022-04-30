#include "minishell.h"

int	env_strs_len(env_list_t *env)
{
	int	len;

	len = 0;
	while (env)
	{
		len += ft_strlen(env->key) + 1;
		if (env->val)
			len += ft_strlen(env->val) + 2;
		else
			len += 3;
		env = env->next;
	}
	return (len);
}

int	env_fill_array(minishell_t *minishell)
{
	int			i;
	env_list_t	*ptr;
	char		*line;

	line = malloc(env_strs_len(minishell->env_list) * sizeof(char));
	if (!line)
		return (errno);
	ptr = minishell->env_list;
	i = 0;
	while (ptr)
	{
		minishell->env_arr[i] = line;
		ft_strcpy(&line, ptr->key);
		*(line++) = '=';
		if (ptr->val)
			ft_strcpy(&line, ptr->val);
		else
			ft_strcpy(&line, "''");
		*(line++) = '\0';
		ptr = ptr->next;
		i++;
	}
	minishell->env_arr[i] = NULL;
	return (0);
}

int	env_to_array(minishell_t *minishell)
{
	if (minishell->env_arr)
		free(minishell->env_arr);
	if (minishell->env_list_size)
		minishell->env_arr = (char **)
			malloc((minishell->env_list_size + 1) * sizeof(char *));
	else
	{
		minishell->env_arr = NULL;
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

int	env_to_list(minishell_t *minishell)
{
	int			i;
	env_list_t	*ptr;

	minishell->env_list = NULL;
	i = 0;
	minishell->env_list_size = i;
	if (minishell->env_arr[i])
		return (0);
	ptr = new_env_elem(minishell->env_arr[i]);
	if (!ptr)
		return (errno);
	minishell->env_list = ptr;
	++i;
	minishell->env_list_size = i;
	while (minishell->env_arr[i])
	{
		minishell->env_list->next = new_env_elem(minishell->env_arr[i]);
		if (!minishell->env_list->next)
			return (errno);
		minishell->env_list_size = i;
		i++;
	}
}
