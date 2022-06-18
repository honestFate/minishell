#include "minishell.h"

int	make_redirect(t_minishell *minishell, t_redirect **redirect, int index)
{
	int	i;

	i = 0;
	if (!redirect)
		return (M_OK);
	while (redirect[i])
	{
		if (redirect[i]->type == REDIRECT_HEREDOC)
		{
			redirect[i]->fname = heredoc(minishell, redirect[i], index);
			if (!redirect[i]->fname)
				return (M_ERR);
			redirect[i]->fd = open(redirect[i]->fname, O_RDONLY);
		}
		else if (redirect[i]->type == REDIRECT_IN)
			redirect[i]->fd = open(redirect[i]->arg2, O_RDONLY);
		else if (redirect[i]->type == REDIRECT_OUT)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_CREAT, 0644);
		else if (redirect[i]->type == REDIRECT_OUT_APPEND)
			redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (redirect[i]->fd < 0)
			return (M_ERR);
		++i;
	}
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

int	pipeline_set_fd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	index;

	index = 0;
	while (pipe_line)
	{
		if (sighandler_set(HEREDOC_MODE))
			return (M_ERR);
		if (make_redirect(minishell, pipe_line->redirect_in, index)
			|| make_redirect(minishell, pipe_line->redirect_out, index))
			return (M_ERR);
		if (sighandler_set(DEFAULT_MODE))
			return (M_ERR);
		pipe_line = pipe_line->next;
		++index;
	}
	return (M_OK);
}
