/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:58:23 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:58:30 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_list_clear(t_minishell *minishell)
{
	t_env_list	*ptr;

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
	int			i;
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
