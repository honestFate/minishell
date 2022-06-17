#include "minishell.h"

int	env_set_key(t_env_list *elem, char *key)
{
	int		key_len;
	char	*temp;

	key_len = ft_strlen(key) + 1;
	temp = (char *)malloc(key_len * sizeof(char));
	if (!temp)
		return (M_ERR);
	if (elem->key)
		free(elem->key);
	ft_strlcpy(temp, key, key_len);
	elem->key = temp;
	return (M_OK);
}

int	env_set_val(t_env_list *elem, char *val)
{
	if (elem->val)
		free(elem->val);
	elem->val = val;
	ft_putendl_fd(elem->val, 1);
	return (M_OK);
}

t_env_list	*new_env_elem(char *env_var)
{
	int			del_pos;
	t_env_list	*elem;
	char		*env_key;
	char		*env_val;

	del_pos = 0;
	env_val = NULL;
	while (env_var[del_pos] != '=' && env_var[del_pos])
		del_pos++;
	env_key = ft_substr(env_var, 0, del_pos);
	if (!env_key)
		return (NULL);
	if (env_var[del_pos])
	{
		env_val = ft_substr(env_var, del_pos + 1, ft_strlen(env_var));
		if (!env_val)
		{
			free(env_key);
			return (NULL);
		}
	}
	elem = (t_env_list *)malloc(sizeof(t_env_list));
	if (!elem)
		return (NULL);
	elem->next = NULL;
	elem->key = NULL;
	elem->val = NULL;
	if (env_set_key(elem, env_key))
	{
		free(elem);
		elem = NULL;
	}
	else if (env_set_val(elem, env_val))
	{
		free(elem->key);
		free(elem);
		elem = NULL;
	}
	return (elem);
}

void    env_list_clear(t_minishell *minishell)
{
	t_env_list *ptr;

	ptr = minishell->env_list;
	while (ptr)
	{
		minishell->env_list = minishell->env_list->next;
		env_del_elem(ptr);
		ptr = minishell->env_list;
	}
}

void	env_del_elem(t_env_list *env_list)
{
	free(env_list->key);
	if (env_list->val)
		free(env_list->val);
	free(env_list);
}

void	envlist_delone(t_minishell *minishell, char *key)
{
	t_env_list	*ptr;
	t_env_list	*tmp;

	if (minishell->env_list)
	{
		if (!ft_strcmp(minishell->env_list->key, key))
			minishell->env_list = minishell->env_list->next;
		else
		{
			ptr = minishell->env_list;
			while (ptr->next)
			{
				if (!ft_strcmp(ptr->next->key, key))
				{
					tmp = ptr->next;
					ptr->next = ptr->next->next;
					env_del_elem(tmp);
				}
				ptr = ptr->next;
			}
		}
	}
}

int	envlist_add_var(t_minishell *minishell, char **argv)
{
	int	i;
	t_env_list	*elem;

	i = 1;
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
	return (M_OK);
}


void	env_add_back(t_env_list **env_list, t_env_list *new_elem)
{
	t_env_list	*ptr;

	if (!*env_list)
	{
		*env_list = new_elem;
		return ;
	}
	ptr = *env_list;
	while (ptr->next)
		ptr = ptr->next;
	ptr->next = new_elem;
}
