/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:37 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:12:38 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	i;
	int	flag;

	(void)minishell;
	i = 1;
	flag = 0;
	while (pipe_line->argv[i] && !strcmp(pipe_line->argv[i], "-n"))
	{
		++flag;
		++i;
	}
	if (pipe_line->argv[i])
	{
		ft_putstr_fd(pipe_line->argv[i], STDOUT_FILENO);
		++i;
		while (pipe_line->argv[i])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			ft_putstr_fd(pipe_line->argv[i], STDOUT_FILENO);
			++i;
		}
	}
	if (!flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (M_OK);
}
