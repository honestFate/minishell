#include "minishell.h"

int	env_set_key(env_list_t *elem, char *key)
{
	int			key_len;

	key_len = ft_strlen(key) + 1;
	elem->key = (char *)malloc(key_len * sizeof(char));
	if (elem->key)
		ft_strlcpy(elem->key, key, key_len);
	return (errno);
}

int	env_set_val(env_list_t *elem, char *val)
{
	int	val_len;

	val_len = ft_strlen(val) + 1;
	elem->val = (char *)malloc(val_len * sizeof(char));
	if (elem->key)
		ft_strlcpy(elem->key, val, val_len);
	return (errno);
}

env_list_t	*new_env_elem(char *env_var)
{
	env_list_t	*elem;
	char		**env_splited;

	env_splited = ft_split(env_var, '=');
	if (!env_splited)
		return (NULL);
	elem = (env_list_t *)malloc(sizeof(env_list_t));
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

void    env_list_clear(minishell_t *minishell)
{
	while (minishell->env_list)
		minishell->env_list = env_del_elem(minishell->env_list);
}

env_list_t	*env_del_elem(env_list_t *env_list)
{
	env_list_t	*ptr;

	ptr = env_list->next;
	free(env_list->key);
	if (env_list->val)
		free(env_list->val);
	free(env_list);
	return (ptr);
}

int	envlist_add_var(minishell_t *minishell, char **argv)
{
	int	i;
	env_list_t	*elem;

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


void	env_add_back(env_list_t **env_list, env_list_t *new_elem)
{
	env_list_t	*ptr;

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