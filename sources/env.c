/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:51 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:12:52 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_minishell *minishell, t_pipe_line *pipe_line)
{
	t_env_list	*ptr;

	if (pipe_line->argv[1])
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
