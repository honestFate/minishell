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
	char		*env_splited;

	env_splited = ft_split(env_var, '=');
	if (!env_splited)
		return (errno);
	elem = (env_list_t *)malloc(sizeof(env_list_t));
	if (!elem)
		return (NULL);	
	if (env_set_key(elem, env_splited[0]))
		free(elem);
	else if (env_splited[1] && env_set_val(elem, env_splited[1]))
	{
		free(elem->key);
		free(elem);
	}
	return (elem);
}

void    env_list_clear(minishell_t *shell)
{
    env_list_t	*ptr;

	while (shell->env_list)
	{
		ptr = shell->env_list->next;
		free(shell->env_list->key);
		if (shell->env_list->val)
			free(shell->env_list->val);
		free(shell->env_list);
		shell->env_list = ptr;
	}
}
