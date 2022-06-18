#include "minishell.h"

int	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line, int fd_in, int fd_out)
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
		return (minishell->built_in[built_in](minishell, pipe_line->argv));
	err = find_cmd(pipe_line->cmd, minishell->env_list, &path_to_cmd);
	if (path_to_cmd == NULL)
		fatal_err(minishell, pipe_line, err);
	envarr_change_val(minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
	fatal_err(minishell, pipe_line, errno);
}

int	exec_in_fork(t_minishell *minishell, t_pipe_line *pipe_line, t_pipe_desc *pipe_desc)
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
		exec_cmd(minishell, pipe_line, pipe_desc->fd_in, pipe_desc->fd_out);
		fatal_err(minishell, pipe_line, errno);
	}
	return (M_ERR);
}
