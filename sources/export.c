/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:13:05 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:42:24 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	export_new_val(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	i;
	int	err;

	i = 1;
	err = M_OK;
	while (pipe_line->argv[i])
	{
		if (envvar_validate(pipe_line->argv[i]))
		{
			err = M_ERR;
			print_error(
				pipe_line->argv[0],
				INVALID_IDENTIFER,
				pipe_line->argv[i]);
		}
		if (envlist_add_var(minishell, pipe_line->argv))
			return (M_ERR);
		++i;
	}
	return (err);
}

int	ft_export(t_minishell *minishell, t_pipe_line *pipe_line)
{
	t_env_list	*ptr;

	if (pipe_line->argv[1])
		return (export_new_val(minishell, pipe_line));
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
