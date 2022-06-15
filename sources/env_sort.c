#include "minishell.h"

void select_sort(t_env_list *ptr)
{
	t_env_list	*i;
	t_env_list	*j;
	t_env_list	*pos;
	char	*temp_key;
	char	*temp_val;

	if (!ptr)
		return ;
	i = ptr;
	while (i->next)
	{
		pos = i;
		temp_key = i->key;
		temp_val = i->val;
		j = i->next;
		while(j)
		{
			if (ft_strcmp(j->key, temp_key) < 0)
			{
				pos = j;
				temp_key = j->key;
				temp_val = j->val;
			}
			j = j->next;
		}
		pos->key = i->key;
		pos->val = i->val;
		i->key = temp_key;
		i->val = temp_val;
		i = i->next;
	}
}
