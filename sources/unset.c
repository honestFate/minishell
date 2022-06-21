/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:13:32 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:21:43 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_unset(t_minishell *minishell, t_pipe_line *pipe_line)
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
			print_error(pipe_line->argv[0], INVALID_IDENTIFER,
				pipe_line->argv[i]);
		}
		else
			envlist_delone(minishell, pipe_line->argv[i]);
		++i;
	}
	return (err);
}
