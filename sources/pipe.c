/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:13:17 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:36:09 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_pipe_desc	*pipe_desc_init(
	int exec_type,
	int fd_in,
	int fd_out,
	int fd_to_close)
{
	t_pipe_desc	*pipe_desc;

	pipe_desc = (t_pipe_desc *)malloc(sizeof(t_pipe_desc));
	if (pipe_desc)
	{
		pipe_desc->exec_type = exec_type;
		pipe_desc->fd_in = fd_in;
		pipe_desc->fd_out = fd_out;
		pipe_desc->fd_to_close = fd_to_close;
	}
	return (pipe_desc);
}

static void	next_pipe(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc,
	int p[2])
{
	if (pipe_line->next->next)
	{
		pipe_exec(minishell, pipe_line->next,
			pipe_desc_init(PIPE, p[READ_END], pipe_desc->fd_out, -1));
	}
	else
	{
		pipe_exec(minishell, pipe_line->next,
			pipe_desc_init(SIMPLE, p[READ_END], pipe_desc->fd_out, -1));
		if (safe_close(p[READ_END]))
			fatal_err(minishell, pipe_line, errno);
	}	
}

int	pipe_exec(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc)
{
	int	p[2];

	if (pipe_desc->exec_type == PIPE)
	{
		if (pipe(p))
			fatal_err(minishell, pipe_line, errno);
		pipe_exec(minishell, pipe_line,
			pipe_desc_init(
				SIMPLE,
				pipe_desc->fd_in,
				p[WRITE_END],
				p[READ_END]));
		if (safe_close(p[WRITE_END]) || safe_close(pipe_desc->fd_in))
			fatal_err(minishell, pipe_line, errno);
		next_pipe(minishell, pipe_line, pipe_desc, p);
		free(pipe_desc);
	}
	else if (exec_in_fork(minishell, pipe_line, pipe_desc))
		fatal_err(minishell, pipe_line, errno);
	return (M_OK);
}
