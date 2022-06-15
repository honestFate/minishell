#include "minishell.h"

int	envlist_change_val(t_env_list *env_list, char *key, char *new_val)
{
	while (env_list)
	{
		if (!strcmp(env_list->key, key))
		{
			free(env_list->val);
			return (env_set_val(env_list, new_val));
		}
		env_list = env_list->next;
	}
	return (M_ERR);
}

int	envarr_change_val(char **env, char *key, char *val)
{
	int		i;
	char	*tmp;

	i = 0;
	while (env[i])
	{
		if (!ft_strcmp(env[i], key))
		{
			free(env[i]);
			tmp = ft_strjoin(key, "=");
			if (!tmp)
				return (M_ERR);
			env[i] = ft_strjoin(tmp, val);
			free(tmp);
			if (!env[i])
				return (M_ERR);
			return (M_OK);
		}
		++i;
	}
	return (M_ERR);
}

char	*ft_getenv(t_env_list *env_list, char *name)
{
	while (env_list && ft_strcmp(env_list->key, name))
		env_list = env_list->next;
	return (env_list->val);
}
