#include "minishell.h"

int	ft_export(t_minishell *minishell, char **argv)
{
	int			i;
	int			err;
	t_env_list	*ptr;

	i = 1;
	err = 0;
	if (argv[i])
	{
		while (argv[i])
		{
			if (envvar_validate(argv[i]))
			{
				err = M_ERR;
				print_error(argv[0], INVALID_IDENTIFER, argv[i]);
			}
			if (envlist_add_var(minishell, argv))
				return (M_ERR);
			++i;
		}
		return (err);
	}
	select_sort(minishell->env_list);
	ptr = minishell->env_list;
	while (ptr)
	{
		if (ft_strcmp(ptr->key, "_") != 0)
			printf("declare -x %s=\"%s\"\n", ptr->key, ptr->val);
		ptr = ptr->next;
	}
	return (M_OK);
}
