#include "minishell.h"

int	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line, int fd_in, int fd_out)
{
	int		err;
	int		built_in;
	char	*path_to_cmd;

	path_to_cmd = NULL;
	if (fd_in >= 0)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0 || close(fd_in))
			return (M_ERR);
	}
	if (fd_out >= 0)
	{
		if (dup2(fd_out, STDOUT_FILENO) < 0 || close(fd_out))
			return (M_ERR);
	}
	if (cmd_redirect(pipe_line->redirect_in)
		|| cmd_redirect(pipe_line->redirect_out))
		return (M_ERR);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		return (minishell->built_in[built_in](minishell, pipe_line->argv));
	err = find_cmd(pipe_line->cmd, minishell->env_list, &path_to_cmd);
	if (path_to_cmd == NULL)
	{
		//printf("find errno - %s\n", strerror(errno));
		fatal_err(minishell, pipe_line, err);
	}
	envarr_change_val(minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
	/*if (errno)
		printf("exec errno - %s\n", strerror(errno));*/
	exit(errno);
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

t_pipe_desc *pipe_desc_init(int exec_type, int fd_in, int fd_out, int fd_to_close)
{
	t_pipe_desc *pipe_desc;
	
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

int	pipe_exec(t_minishell *minishell, t_pipe_line *pipe_line, t_pipe_desc *pipe_desc)
{
	int	p[2];

	if (pipe_desc->exec_type == PIPE)
	{
		if (pipe(p))
			fatal_err(minishell, pipe_line, errno);
		pipe_exec(minishell, pipe_line,
			pipe_desc_init(SIMPLE, pipe_desc->fd_in, p[WRITE_END], p[READ_END]));
		if (safe_close(p[WRITE_END])
			|| safe_close(pipe_desc->fd_in))
			fatal_err(minishell, pipe_line, errno);
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
		free(pipe_desc);
	}
	else if (exec_in_fork(minishell, pipe_line, pipe_desc))
		fatal_err(minishell, pipe_line, errno);
	return (M_OK);
}

int exec_pipe_line(t_minishell *minishell, t_pipe_line *pipe_line)
{
	//int			pipe_len;
	t_std_backup std_backup;
	int			exit_status;
	t_pipe_line	*ptr;

	exit_status = -1;
	if (stdbackup_copy(&std_backup))
		fatal_err(minishell, pipe_line, errno);
	if (pipeline_set_fd(minishell, pipe_line))
	{
		if (isatty(STDIN_FILENO) || stdbackup_set(&std_backup))
			fatal_err(minishell, pipe_line, errno);
	}
	if (!pipe_line->next && is_builtin(pipe_line->cmd) >= 0)
		g_exit_status = exec_cmd(minishell, pipe_line, -1, -1);
	else
	{
		if (pipe_line->next)
			pipe_exec(minishell, pipe_line, pipe_desc_init(PIPE, -1, -1, -1));
		else
			pipe_exec(minishell, pipe_line, pipe_desc_init(SIMPLE, -1, -1, -1));
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
	}
	if (stdbackup_set(&std_backup) || stdbackup_close(&std_backup))
		fatal_err(minishell, pipe_line, errno);
	printf("%d\n", exit_status);
	sighandler_set(DEFAULT_MODE);
	if (exit_status >= 0)
	{
		if (WIFEXITED(exit_status) && !WEXITSTATUS(exit_status))
		{
			printf("Успешное завершение\n");
			g_exit_status = 0;
		}
		else
		{
			printf("Не ноль\n");
			if (WIFSIGNALED(exit_status))
			{
				printf("Сигнал - %d\n", WTERMSIG(exit_status));
				g_exit_status = WTERMSIG(exit_status) + 128;
			}
			else
			{
				printf("Не сигнал - %d\n", WEXITSTATUS(exit_status));
				g_exit_status = WEXITSTATUS(exit_status);
			}
		}
	}
	return (g_exit_status);
}

//TEST
int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	g_exit_status = 0;
	t_minishell minishell = {.exit_status = 0};
	env_copy(&minishell, envp);
	if (env_to_list(&minishell, envp))
	{
		printf("ENV TO LIST FAILED %s\n", NULL);
		exit(2);
	}
	default_env(&minishell);
	builtin_arr_init(&minishell);
	char *cmd_line;
	char *cmd_argv;
	char *redirect_in;
	char *redirect_out;
	char *is_pipe;
	t_pipe_line *pipe_line = NULL;
	t_pipe_line *temp;
	int i = 0;
	sighandler_set(DEFAULT_MODE);
	//sighandler_set(HEREDOC_MODE);
	//sighandler_set(DEFAULT_MODE);
	while (1)
	{
		is_pipe = (char *)1;
		while (is_pipe)
		{
			i = 0;
			cmd_line = readline("> ");
			if (!cmd_line)
			{
				printf("exit\n");
				return (M_OK);
			}
			cmd_argv = readline("argv> ");
			redirect_in = readline("redirect in [from to type] ");
			redirect_out = readline("redirect out [from to type] ");
			is_pipe = readline("is pipe?");
			temp = (t_pipe_line *)malloc(sizeof(t_pipe_line));
			temp->cmd = cmd_line;
			if (*cmd_argv)
			{
				char *temp_argv = ft_strjoin(cmd_line, " ");
				char *temp_argv_1 = ft_strjoin(temp_argv, cmd_argv);
				safe_free(temp_argv);
				temp->argv = ft_split(temp_argv_1, ' ');
				safe_free(temp_argv_1);
			}
			else
			{
				temp->argv = ft_split(cmd_line, ' ');
			}
			write(1, "ok\n", 3);
			if (*redirect_in)
				temp->redirect_in = (t_redirect **)malloc(sizeof(t_redirect *) * 2);
			else
				temp->redirect_in = NULL;
			if (*redirect_out)
				temp->redirect_out = (t_redirect **)malloc(sizeof(t_redirect *) * 2);
			else
				temp->redirect_out = NULL;
			if (*redirect_in)
			{
				write(1, "ok\n", 3);
				char **redirect_in_splited = ft_split(redirect_in, ' ');
				temp->redirect_in[i] = (t_redirect *)malloc(sizeof(t_redirect));
				temp->redirect_in[i]->arg1 = ft_atoi(redirect_in_splited[1]);
				temp->redirect_in[i]->arg2 = ft_strdup(redirect_in_splited[0]);
				temp->redirect_in[i]->type = ft_atoi(redirect_in_splited[2]);
				temp->redirect_in[i]->fname = NULL;
				i++;
				temp->redirect_in[i] = NULL;
				free_str_arr(redirect_in_splited);
			}
			i = 0;
			if (*redirect_out)
			{
				write(1, "ok\n", 3);
				char **redirect_out_splited = ft_split(redirect_out, ' ');
				temp->redirect_out[i] = (t_redirect *)malloc(sizeof(t_redirect));
				temp->redirect_out[i]->arg1 = ft_atoi(redirect_out_splited[0]);
				temp->redirect_out[i]->arg2 = ft_strdup(redirect_out_splited[1]);
				temp->redirect_out[i]->type = ft_atoi(redirect_out_splited[2]);
				temp->redirect_out[i]->fname = NULL;
				i++;
				temp->redirect_out[i] = NULL;
				free_str_arr(redirect_out_splited);
			}
			if (!pipe_line)
			{
				pipe_line = temp;
				pipe_line->prev = NULL;
			}
			else
			{
				temp->prev = pipe_line;
				pipe_line->next = temp;
				pipe_line = pipe_line->next;
			}
			pipe_line->next = NULL;
			if (!*is_pipe)
				break;
		}
		write(1, "ok\n", 3);
		while (pipe_line->prev)
			pipe_line = pipe_line->prev;
		int exit_status = exec_pipe_line(&minishell, pipe_line);
		write(1, "free start\n", 11);
		free_pipe_line(pipe_line);
		write(1, "free end\n", 9);
		pipe_line = NULL;
		//safe_free(cmd_line);
		//safe_free(redirect_in);
		//safe_free(redirect_out);
		safe_free(is_pipe);
		printf("exit status is %d\n", exit_status);
	}
}
//END TEST
