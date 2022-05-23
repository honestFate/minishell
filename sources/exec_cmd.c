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
	cmd2.redirect_out = (t_redirect *)malloc(sizeof(t_redirect));
	cmd2.redirect_out[0].arg1 = 1;
	cmd2.redirect_out[0].arg2 = "test1";
	cmd2.redirect_out[0].fd = 0;
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

int	make_redirect_in(t_redirect *redirect_in)
{
	int	i;

	i = 0;
	while (redirect_in[i])
	{

		++i;
	}
}

void	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	built_in;

	make_redirect(pipe_line->redirect_in);
	make_redirect(pipe_line->redirect_out);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		minishell->built_in[built_in](minishell, pipe_line->argv);
	
}

int	pipe_exec(t_minishell *minishell, t_pipe_line *pipe_line, int type, int fd_in, int fd_out)
{
	int	p[2];

	if (type == PIPE)
	{
		pipe(p);
		pipe_exec(minishell, pipe_line, SIMPLE, fd_in, p[WRITE_END]);
		if (pipe_line->next->next)
			pipe_exec(minishell, pipe_line->next, PIPE, p[READ_END], STDOUT_FILENO);
		else
			pipe_exec(minishell, pipe_line->next, SIMPLE, p[READ_END], STDOUT_FILENO);
	}
	else if (type == SIMPLE)
	{
		pipe_line->pid = fork();
		if (pipe_line->pid > 0)
			return (M_OK);
		if (pipe_line->pid == 0)
			exec_cmd(minishell, pipe_line);
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
		exec_cmd(minishell, pipe_line);
	ptr = pipe_line;
	while (ptr)
	{
		if (ptr->pid > 0)
			waitpid(ptr->pid, &exit_status, WUNTRACED);
		ptr = ptr->next;
	}
	return (exit_status);
}
