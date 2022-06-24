/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:56 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/23 06:10:33 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*f_path(t_params *data, t_node *pipe_line)
{
	int		err;
	char	*path_to_cmd;

	err = find_cmd(pipe_line->cmd, data->minishell->env_list, &path_to_cmd);
	if (err == EACCES)
	{
		//free_strarr_terminated(argv);
		print_error(pipe_line->cmd, EACCES, NULL);
		exit_minishell(data, 126);
	}
	if (err == ENOENT)
	{
		//free_strarr_terminated(argv);
		print_error(pipe_line->cmd, ENOENT, NULL);
		exit_minishell(data, 127);
	}
	return (path_to_cmd);
}

int	exec_cmd(t_params *data, t_node *pipe_line,
	int fd_in, int fd_out)
{
	//char	**new_argv;
	int		err;
	//char	**tmp;
	char	*path_to_cmd;

	path_to_cmd = NULL;
	//printf("%d %d %d\n", pipe_line->need_to_free_str, pipe_line->n_arg, pipe_line->need_to_free_str);
	pipe_line->arg = argv_crutch(pipe_line->arg, pipe_line->cmd,
			pipe_line->arg_count);
	pipe_line->arg_count++;
	if (!pipe_line->need_to_free_arg) 
		pipe_line->need_to_free_arg++;
	if (redirect_pipe(fd_in, fd_out) || cmd_redirect_s(pipe_line->rdir))
	{
		//free_strarr_terminated(new_argv);
		return (M_ERR);
	}
	if (is_builtin(pipe_line->cmd) >= 0)
	{
		/*tmp = pipe_line->arg;
		pipe_line->arg = new_argv;*/
		err = data->minishell->
			built_in[is_builtin(pipe_line->cmd)](data, pipe_line);
		//pipe_line->arg = tmp;
		//free_strarr_terminated(new_argv);
		return (err);
	}
	path_to_cmd = f_path(data, pipe_line);
	envarr_change_val(data->minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->arg, data->minishell->env_arr);
	free(path_to_cmd);
	print_error(pipe_line->cmd, errno, NULL);
	exit_minishell(data, M_ERR);
	return (M_ERR);
}

int	exec_in_fork(t_params *data, t_node *pipe_line,
	t_pipe_desc *pipe_desc)
{
	int	err;

	pipe_line->pid = fork();
	if (pipe_line->pid > 0)
	{
		sighandler_set(EXEC_MODE_PARENT);
		if (cmd_redirect_close(pipe_line->rdir))
			return (M_ERR);
		return (M_OK);
	}
	else if (pipe_line->pid == 0)
	{
		errno = 0;
		sighandler_set(EXEC_MODE_CHILD);
		safe_close(pipe_desc->fd_to_close);
		err = exec_cmd(data, pipe_line, pipe_desc->fd_in,
				pipe_desc->fd_out);
		exit_minishell(data, err);
	}
	else
	{
		print_error(pipe_line->cmd, errno, NULL);
		exit_minishell(data, M_ERR);
	}
	return (M_ERR);
}
