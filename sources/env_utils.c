#include "minishell.h"

int	env_set_key(t_env_list *elem, char *key)
{
	int		key_len;
	char	*temp;

	key_len = ft_strlen(key) + 1;
	temp = (char *)malloc(key_len * sizeof(char));
	if (temp)
	{
		if (!elem->key)
			free(elem->key);
		ft_strlcpy(temp, key, key_len);
		elem->key = temp;
	}
	return (errno);
}

int	env_set_val(t_env_list *elem, char *val)
{
	int		val_len;
	char	*temp;

	val_len = ft_strlen(val) + 1;
	temp = (char *)malloc(val_len * sizeof(char));
	if (temp)
	{
		if (!elem->val)
			free(elem->val);
		ft_strlcpy(temp, val, val_len);
		elem->val = temp;
	}
	return (errno);
}

t_env_list	*new_env_elem(char *env_var)
{
	t_env_list	*elem;
	char		**env_splited;

	env_splited = ft_split(env_var, '=');
	if (!env_splited)
		return (NULL);
	elem = (t_env_list *)malloc(sizeof(t_env_list));
	if (!elem)
		return (NULL);
	elem->next = NULL;	
	if (env_set_key(elem, env_splited[0]))
		free(elem);
	else if (env_splited[1] && env_set_val(elem, env_splited[1]))
	{
		free(elem->key);
		free(elem);
		return (NULL);
	}
	return (elem);
}

void    env_list_clear(t_minishell *minishell)
{
	while (minishell->env_list)
		minishell->env_list = env_del_elem(minishell->env_list);
}

t_env_list	*env_del_elem(t_env_list *env_list)
{
	t_env_list	*ptr;

	ptr = env_list->next;
	free(env_list->key);
	if (env_list->val)
		free(env_list->val);
	free(env_list);
	return (ptr);
}

int	envlist_add_var(t_minishell *minishell, char **argv)
{
	int	i;
	t_env_list	*elem;

	i = 0;
	while (argv[i])
	{
		elem = new_env_elem(argv[i]);
		if (!elem)
		{
			env_list_clear(minishell);
			return (errno);
		}
		env_add_back(&minishell->env_list, elem);
		++i;
	}
	return (0);
}


void	env_add_back(t_env_list **env_list, t_env_list *new_elem)
{
	t_env_list	*ptr;

	if (!env_list)
	{
		*env_list = new_elem;
		return ;
	}
	ptr = *env_list;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new_elem;
}