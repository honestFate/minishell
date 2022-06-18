#include "minishell.h"

int	ft_env(t_minishell *minishell, char **argv)
{
	t_env_list	*ptr;

	if (argv[1])
		return (USAGE_ERROR);
	ptr = minishell->env_list;
	while (ptr)
	{
		if (ptr->val)
		{
			ft_putstr_fd(ptr->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(ptr->val, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		ptr = ptr->next;
	}
	return (M_OK);
}
