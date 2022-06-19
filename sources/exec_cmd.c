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
	if (redirect_pipe(fd_in, fd_out))
		return (M_ERR);
	if (cmd_redirect(pipe_line->redirect_in)
		|| cmd_redirect(pipe_line->redirect_out))
		return (M_ERR);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		return (minishell->built_in[built_in](minishell, pipe_line));
	err = find_cmd(pipe_line->cmd, minishell->env_list, &path_to_cmd);
	ft_putstr_fd("find err - ", STDERR_FILENO);
	ft_putnbr_fd(err, STDERR_FILENO);
	if (err)
		exit_minishell(minishell, pipe_line, err, NULL);
	envarr_change_val(minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
	exit_minishell(minishell, pipe_line, errno, NULL);
	return (M_ERR);
}

int	exec_in_fork(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc)
{
	int	err;

	pipe_line->pid = fork();
	if (pipe_line->pid > 0)
	{
		sighandler_set(EXEC_MODE_PARENT);
		if (cmd_redirect_close(pipe_line->redirect_in)
			|| cmd_redirect_close(pipe_line->redirect_out))
			return (M_ERR);
		ft_putendl_fd("parrent fork ok", STDERR_FILENO);
		return (M_OK);
	}
	else if (pipe_line->pid == 0)
	{
		errno = 0;
		sighandler_set(EXEC_MODE_CHILD);
		safe_close(pipe_desc->fd_to_close);
		ft_putendl_fd("child go exec", STDERR_FILENO);
		err = exec_cmd(minishell,
				pipe_line, pipe_desc->fd_in, pipe_desc->fd_out);
		ft_putendl_fd("child is not ok", STDERR_FILENO);
		exit_minishell(minishell, pipe_line, err, NULL);
	}
	return (M_ERR);
}
