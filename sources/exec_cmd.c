#include "minishell.h"

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

int	make_redirect(t_minishell *minishell, t_redirect **redirect, int index)
{
	int	i;

	i = 0;
	if (!redirect)
		return (M_OK);
	ft_putendl_fd("redirect start", 2);
	while (redirect[i])
	{
		ft_putendl_fd(redirect[i]->arg2, 2);
		if (redirect[i]->type == REDIRECT_HEREDOC)
		{
			redirect[i]->fname = heredoc(minishell, 1, redirect[i]->arg2, index);
			if (!redirect[i]->fname)
				return (M_ERR);
			ft_putendl_fd(redirect[i]->fname, 1);
			redirect[i]->fd = open(redirect[i]->fname, O_RDONLY);
		}
		else if (redirect[i]->type == REDIRECT_IN)
			redirect[i]->fd = open(redirect[i]->arg2, O_RDONLY);
		else if (redirect[i]->type == REDIRECT_OUT)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_CREAT, 0644);
		else if (redirect[i]->type == REDIRECT_OUT_APPEND)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (redirect[i]->fd < 0)
		{
			ft_putendl_fd("redirect error", 2);
			ft_putendl_fd(strerror(errno), 2);
			return (M_ERR);
		}
		//close(redirect[i]->arg1);
		++i;
	}
	ft_putendl_fd("redirect OK", 2);
	return (M_OK);
}

int	cmd_redirect(t_redirect **redirect_arr)
{
	int	i;

	i = 0;
	if (!redirect_arr)
		return (M_OK);
	while (redirect_arr[i])
	{
		if (dup2(redirect_arr[i]->fd, redirect_arr[i]->arg1) < 0)
			return (M_ERR);
		if (close(redirect_arr[i]->fd))
			return (M_ERR);
		++i;
	}
	return (M_OK);
}

int	cmd_redirect_close(t_redirect **redirect_arr)
{
	int	i;

	i = 0;
	if (!redirect_arr)
		return (M_OK);
	while (redirect_arr[i])
	{
		if (close(redirect_arr[i]->fd))
			return (M_ERR);
		redirect_arr[i]->fd = -1;
		++i;
	}
	return (M_OK);
}

int	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line, int fd_in, int fd_out)
{
	int		built_in;
	char	*path_to_cmd;

	path_to_cmd = NULL;
	if (fd_in >= 0)
	{
		if (dup2(fd_in, STDIN_FILENO) < 0)
			return (M_ERR);
		close(fd_in);
	}
	if (fd_out >= 0)
	{
		if (dup2(fd_out, STDOUT_FILENO) < 0)
			return (M_ERR);
		close(fd_out);
	}
	ft_putendl_fd(pipe_line->cmd, 1);
	if (cmd_redirect(pipe_line->redirect_in)
		|| cmd_redirect(pipe_line->redirect_out))
		return (M_ERR);
	built_in = is_builtin(pipe_line->cmd);
	if (built_in >= 0)
		return (minishell->built_in[built_in](minishell, pipe_line->argv));
	path_to_cmd = find_cmd(pipe_line->cmd, minishell->env_list);
	if (path_to_cmd == NULL)
	{
		printf("find errno - %s\n", strerror(errno));
		fatal_err(minishell, pipe_line);
	}
	envarr_change_val(minishell->env_arr, "_", path_to_cmd);
	execve(path_to_cmd, pipe_line->argv, minishell->env_arr);
	if (errno)
		printf("exec errno - %s\n", strerror(errno));
	return (errno);
}

int safe_close(int fd)
{
	if (fd >= 0)
		return (close(fd));
	return (M_OK);
}

int	std_copy(t_std_backup *std_backup)
{
	std_backup->stdin_backup = dup(STDIN_FILENO);
	if (std_backup->stdin_backup >= 0)
	{
		std_backup->stdout_backup = dup(STDIN_FILENO);
		if (std_backup->stdout_backup >= 0)
		{
			std_backup->stderr_backup = dup(STDIN_FILENO);
			if (std_backup->stderr_backup < 0)
			{
				close(std_backup->stdin_backup);
				close(std_backup->stdout_backup);
			}
		}
		else
			close(std_backup->stdin_backup);
	}
	return (errno);
}

int std_set(t_std_backup *std_backup)
{
	if (dup2(std_backup->stdin_backup, STDIN_FILENO) < 0
		|| dup2(std_backup->stdout_backup, STDOUT_FILENO) < 0
		|| dup2(std_backup->stderr_backup, STDERR_FILENO) < 0)
		return (M_ERR);
	if (safe_close(std_backup->stdin_backup)
		|| safe_close(std_backup->stdout_backup)
		|| safe_close(std_backup->stderr_backup))
		return (M_ERR);
	return (M_OK);
}

int	exec_in_fork(t_minishell *minishell, t_pipe_line *pipe_line, t_pipe_desc *pipe_desc)
{
	pipe_line->pid = fork();
	if (pipe_line->pid > 0)
	{
		ft_putendl_fd("MAIN", 1);
		ft_putendl_fd(pipe_line->cmd, 1);
		if (cmd_redirect_close(pipe_line->redirect_in)
			|| cmd_redirect_close(pipe_line->redirect_out))
			return (M_ERR);
		return (M_OK);
	}
	if (pipe_line->pid == 0)
	{
		safe_close(pipe_desc->fd_to_close);
		exec_cmd(minishell, pipe_line, pipe_desc->fd_in, pipe_desc->fd_out);
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
			fatal_err(minishell, pipe_line);
		if (pipe_exec(minishell, pipe_line,
			pipe_desc_init(SIMPLE, pipe_desc->fd_in, p[WRITE_END], p[READ_END])))
			fatal_err(minishell, pipe_line);
		if (safe_close(p[WRITE_END])
			|| safe_close(pipe_desc->fd_in))
			fatal_err(minishell, pipe_line);
		if (pipe_line->next->next)
		{
			if (pipe_exec(minishell, pipe_line->next,
				pipe_desc_init(PIPE, p[READ_END], pipe_desc->fd_out, -1)))
				fatal_err(minishell, pipe_line);
		}
		else
		{
			if (pipe_exec(minishell, pipe_line->next,
				pipe_desc_init(SIMPLE, p[READ_END], pipe_desc->fd_out, -1)))
				fatal_err(minishell, pipe_line);
			safe_close(p[READ_END]);
		}
		free(pipe_desc);
	}
	else if (exec_in_fork(minishell, pipe_line, pipe_desc))
		fatal_err(minishell, pipe_line);
	return (M_OK);
}

int	pipeline_set_fd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	i;

	i = 0;
	while (pipe_line)
	{
		if (make_redirect(minishell, pipe_line->redirect_in, i)
			|| make_redirect(minishell, pipe_line->redirect_out, i))
			return (M_ERR);
		//printf("in - %d, out - %d", pipe_line->redirect_in[0]->fd, pipe_line->redirect_out[0]->fd);
		pipe_line = pipe_line->next;
		++i;
	}
	return (M_OK);
}

int exec_pipe_line(t_minishell *minishell, t_pipe_line *pipe_line)
{
	//int			pipe_len;
	t_std_backup std_backup;
	int			exit_status;
	t_pipe_line	*ptr;

	exit_status = 0;
	if (pipeline_set_fd(minishell, pipe_line))
		fatal_err(minishell, pipe_line);
	if (!pipe_line->next && is_builtin(pipe_line->cmd) >= 0)
	{
		if (std_copy(&std_backup))
			fatal_err(minishell, pipe_line);
		exit_status = exec_cmd(minishell, pipe_line, -1, -1);
		if (std_set(&std_backup))
			fatal_err(minishell, pipe_line);
	}
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
	//free_pipe_line(pipe_line);
	//sleep(100000);
	return (exit_status);
}

void	safe_free(void *data)
{
	if (data)
		free(data);
}

//TEST
int main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	sighandler_set(0);
	t_minishell minishell = {.exit_status = 0};
	env_copy(&minishell, envp);
	if (env_to_list(&minishell, envp))
	{
		printf("ENV TO LIST FAILED %s\n", NULL);
		exit(2);
	}
	builtin_arr_init(&minishell);
	char *cmd_line;
	char *cmd_argv;
	char *redirect_in;
	char *redirect_out;
	char *is_pipe;
	t_pipe_line *pipe_line = NULL;
	t_pipe_line *temp;
	int i = 0;
	while (1)
	{
		is_pipe = (char *)1;
		while (is_pipe)
		{
			i = 0;
			cmd_line = readline("> ");
			if (!cmd_line)
				return (M_OK);
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
