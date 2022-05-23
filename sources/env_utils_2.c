#include "minishell.h"

int	env_change_val(t_env_list *env_list, char *key, char *new_val)
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

char	*ft_getenv(t_env_list *env_list, char *name)
{
	while (env_list && ft_strcmp(env_list->key, name))
		env_list = env_list->next;
	return (env_list);
}
