#include "minishell.h"

char	*ft_strerr(int error)
{
	if (error <= 255)
		return (strerror(error));
	if (error == TOO_MANY_ARGS)
		return ("слишком много аргументов");
}

void	print_error(char *cmd, int error)
{
	ft_putendl_fd("minishell: ", STDERR_FILENO);
	ft_putendl_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(": ", STDERR_FILENO);
	ft_putendl_fd(ft_strerr(error), STDERR_FILENO);
}

void	free_redirect(t_redirect **redirect)
{
	int	i;

	i = 0;
	while (redirect[i])
	{
		free(redirect[i]->arg2);
		free(redirect[i]);
	}
	free(redirect);
}

void	free_pipe_line(t_pipe_line *pipe_line)
{
	t_pipe_line	*ptr;

	while (pipe_line->next)
		pipe_line = pipe_line->next;
	while (pipe_line)
	{
		ptr = pipe_line->prev;
		free_redirect(pipe_line->redirect_in);
		free_redirect(pipe_line->redirect_out);
		free(pipe_line->cmd);
		free_str_arr(pipe_line->argv);
		free(pipe_line);
		pipe_line = ptr;
	}
}

void	fatal_err(t_minishell *minishell, t_pipe_line *pipe_line)
{
	free_pipe_line(pipe_line);
	free_str_arr(minishell->env_arr);
	env_list_clear(minishell->env_list);
	free(minishell);
}
