/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:56 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 05:46:25 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_cmd(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	int fd_in,
	int fd_out)
{
	int		err;
	int		built_in;
	char	*path_to_cmd;

	path_to_cmd = NULL;
	//pipe_line->argv = argv_crutch(pipe_line->argv, pipe_line->cmd, 0);
	if (redirect_pipe(fd_in, fd_out))
		return (M_ERR);
	if (cmd_redirect(pipe_line->redirect_in)
		|| cmd_redirect(pipe_line->redirect_out))
		return (M_ERR);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		return (minishell->built_in[built_in](minishell, pipe_line));
	err = find_cmd(pipe_line->cmd, minishell->env_list, &path_to_cmd);
	if (err == EACCES)
	{
		print_error(pipe_line->cmd, EACCES, NULL);
		exit_minishell(minishell, pipe_line, 126);
	}
	if (err == ENOENT)
	{
		print_error(pipe_line->cmd, ENOENT, NULL);
		exit_minishell(minishell, pipe_line, 127);
	}
	envarr_change_val(minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
	print_error(pipe_line->cmd, errno, NULL);
	exit_minishell(minishell, pipe_line, M_ERR);
	return (M_ERR);
}

int	exec_in_fork(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc)
{
	pipe_line->pid = fork();
	if (pipe_line->pid > 0)
	{
		sighandler_set(EXEC_MODE_PARENT);
		if (cmd_redirect_close(pipe_line->redirect_in)
			|| cmd_redirect_close(pipe_line->redirect_out))
			return (M_ERR);
		return (M_OK);
	}
	else if (pipe_line->pid == 0)
	{
		errno = 0;
		sighandler_set(EXEC_MODE_CHILD);
		safe_close(pipe_desc->fd_to_close);
		exec_cmd(minishell,
			pipe_line, pipe_desc->fd_in, pipe_desc->fd_out);
		print_error(pipe_line->cmd, errno, NULL);
		exit_minishell(minishell, pipe_line, M_ERR);
	}
	else
	{
		print_error(pipe_line->cmd, errno, NULL);
		exit_minishell(minishell, pipe_line, M_ERR);
	}
	return (M_ERR);
}
