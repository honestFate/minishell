#include "minishell.h"

char	*ft_strerr(int error)
{
	if (error <= 255)
		return (strerror(error));
	if (error == INVALID_IDENTIFER)
		return ("not a valid identifier");
	if (error == NOT_NUM_ARG)
		return ("numeric argument required");
	return ("unknow error");
}

void	print_error(char *cmd, int error, char *arg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (arg)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(ft_strerr(error), STDERR_FILENO);
}

void	redirect_clear(t_redirect **redirect_arr)
{
	int	i;

	i = 0;
	if (!redirect_arr)
		return ;
	while (redirect_arr[i])
	{
		if (redirect_arr[i]->type == REDIRECT_HEREDOC)
		{	
			if (unlink(redirect_arr[i]->fname))
			{
				ft_putendl_fd(ft_strerr(errno), STDERR_FILENO);
				ft_putendl_fd(redirect_arr[i]->fname, 1);
			}
			free(redirect_arr[i]->fname);
		}	
		free(redirect_arr[i]->arg2);
		free(redirect_arr[i]);
		++i;
	}
	free(redirect_arr);
}

void	free_pipe_line(t_pipe_line *pipe_line)
{
	t_pipe_line	*ptr;

	while (pipe_line->next)
		pipe_line = pipe_line->next;
	while (pipe_line)
	{
		ptr = pipe_line->prev;
		if (pipe_line->redirect_in)
			redirect_clear(pipe_line->redirect_in);
		if (pipe_line->redirect_out)
			redirect_clear(pipe_line->redirect_out);
		free(pipe_line->cmd);
		free_str_arr(pipe_line->argv);
		free(pipe_line);
		pipe_line = ptr;
	}
}

void	select_exit_status(int err)
{
	if (err == EXIT_SUCCESS)
		exit(0);
	if (err == BUILTIN_FAIL)
		exit(2);
	if (err == EACCES)
		exit(126);
	if (err == ENOENT)
		exit(127);
}

void	exit_minishell(t_minishell *minishell, t_pipe_line *pipe_line, int err, char *arg)
{
	print_error(pipe_line->cmd, err, arg);
	free_pipe_line(pipe_line);
	free_str_arr(minishell->env_arr);
	env_list_clear(minishell);
	free(minishell);
	select_exit_status(err);
}
