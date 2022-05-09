#include "minishell.h"

void	print_error(int error)
{
	printf("%s\n", strerror(error));
}

char	*ft_getenv(env_list_t *env_list, char *name)
{
	while (env_list && ft_strcmp(env_list->key, name))
		env_list = env_list->next;
	return (env_list);
}
