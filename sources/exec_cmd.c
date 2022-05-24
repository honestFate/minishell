#include "minishell.h"

//TEST
int test(void)
{
	t_pipe_line cmd0 = {.cmd = "ls", .argv = {"-l", NULL}, .redirect_in = NULL};
	t_pipe_line cmd1 = {.cmd = "cat", .argv = {"-e", NULL}, .redirect_in = NULL};
	t_pipe_line cmd2 = {.cmd = "head", .argv = {"-n4", NULL}, .redirect_in = NULL};
	cmd0.next = &cmd1;
	cmd1.next = &cmd2;
	cmd2.next = NULL;
	cmd2.redirect_out = (t_redirect **)malloc(sizeof(t_redirect *));
	cmd2.redirect_out[0] = (t_redirect *)malloc(sizeof(t_redirect));
	cmd2.redirect_out[0]->arg1 = 1;
	cmd2.redirect_out[0]->arg2 = "test1";
	cmd2.redirect_out[0]->fd = 0;
}
//END TEST

int	list_len(t_pipe_line *data)
{
	int	i;

	i = 0;
	while (data)
	{
		data = data->next;
		++i;
	}
	return (i);
}

int	is_builtin(char *cmd)
{
	if (!strcmp(cmd, "echo"))
		return (BIN_ECHO);
	if (!strcmp(cmd, "cd"))
		return (BIN_CD);
	if (!strcmp(cmd, "pwd"))
		return (BIN_PWD);
	if (!strcmp(cmd, "export"))
		return (BIN_EXPORT);
	if (!strcmp(cmd, "unset"))
		return (BIN_UNSET);
	if (!strcmp(cmd, "env"))
		return (BIN_ENV);
	if (!strcmp(cmd, "exit"))
		return (BIN_EXIT);
	return (-1);
}

int	make_redirect(t_redirect **redirect)
{
	int	i;

	i = 0;
	while (redirect[i])
	{
		if (redirect[i]->type == REDIRECT_HEREDOC)
			redirect[i]->fd = heredoc();
		else if (redirect[i]->type == REDIRECT_IN)
			redirect[i]->fd = open(redirect[i]->arg2, O_RDONLY);
		else if (redirect[i]->type == REDIRECT_OUT)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY);
		else if (redirect[i]->type == REDIRECT_OUT_APPEND)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_APPEND);
		if (errno)
			return (errno);
		close(redirect[i]->arg1);
		if (dup2(redirect[i]->fd, redirect[i]->arg1) < 0)
			return (errno);
		++i;
	}
}

void	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line, int fd_in, int fd_out)
{
	int		built_in;
	char	*path_to_cmd;

	if (fd_in >= 0)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0)
			fatal_err(minishell, pipe_line);	
		close(fd_in);
	}
	if (fd_out >= 0)
	{
		if (dup2(fd_out, STDOUT_FILENO))
			fatal_err(minishell, pipe_line);
		close(fd_out);
	}
	if (make_redirect(pipe_line->redirect_in) ||
		make_redirect(pipe_line->redirect_out))
		fatal_err(minishell, pipe_line);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		minishell->built_in[built_in](minishell, pipe_line->argv);
	else
	{
		if (find_cmd(pipe_line->cmd, minishell->env_list, path_to_cmd))
			fatal_err(minishell, pipe_line);
		execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
		fatal_err(minishell, pipe_line);
	}
}

int	pipe_exec(t_minishell *minishell, t_pipe_line *pipe_line, int type, int fd_in, int fd_out)
{
	int	p[2];

	if (type == PIPE)
	{
		pipe(p);
		pipe_exec(minishell, pipe_line, SIMPLE, fd_in, p[WRITE_END]);
		if (pipe_line->next->next)
			pipe_exec(minishell, pipe_line->next, PIPE, p[READ_END], fd_out);
		else
			pipe_exec(minishell, pipe_line->next, SIMPLE, p[READ_END], fd_out);
	}
	else if (type == SIMPLE)
	{
		pipe_line->pid = fork();
		if (pipe_line->pid > 0)
		{
			if (fd_in >= 0)
				close(fd_in);
			if (fd_out >= 0)
				close(fd_out);
			return (M_OK);
		}
		if (pipe_line->pid == 0)
		{
			close(minishell->history_fd);
			exec_cmd(minishell, pipe_line, fd_in, fd_out);
		}
		else
			return (errno);
	}
}

int exec_pipe_line(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int			pipe_len;
	int			exit_status;
	t_pipe_line	*ptr;

	if (!pipe_line->next && is_builtin(pipe_line->cmd))
		exec_cmd(minishell, pipe_line, -1, -1);
	else if (pipe_line->next)
		pipe_exec(minishell, pipe_line, PIPE, -1, -1);
	else
		pipe_exec(minishell, pipe_line, SIMPLE, -1, -1);
	ptr = pipe_line;
	while (ptr)
	{
		if (ptr->pid > 0)
		{
			if (ptr->next)
				waitpid(ptr->pid, NULL, WUNTRACED);
			else
				waitpid(ptr->pid, &exit_status, WUNTRACED);
		}
		ptr = ptr->next;
	}
	return (exit_status);
}
