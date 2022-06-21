/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:13:22 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:25:33 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_rdir(t_minishell *minishell, t_redirect **redirect, int index, int i)
{
	if (redirect[i]->type == REDIRECT_HEREDOC)
	{
		redirect[i]->fname = heredoc(minishell, redirect[i], index);
		if (!redirect[i]->fname)
			return (HEREDOC_ERR);
		redirect[i]->fd = open(redirect[i]->fname, O_RDONLY);
	}
	else if (redirect[i]->type == REDIRECT_IN)
		redirect[i]->fd = open(redirect[i]->arg2, O_RDONLY);
	else if (redirect[i]->type == REDIRECT_OUT)
		redirect[i]->fd = open(redirect[i]->arg2, O_WRONLY | O_CREAT, 0644);
	else if (redirect[i]->type == REDIRECT_OUT_APPEND)
		redirect[i]->fd = open(redirect[i]->arg2,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (M_OK);
}

int	make_redirect(t_minishell *minishell, t_redirect **redirect, int index)
{
	int	i;
	int	err;

	i = 0;
	if (!redirect)
		return (M_OK);
	while (redirect[i])
	{
		err = open_rdir(minishell, redirect, index, i);
		if (err == HEREDOC_ERR)
			return (err);
		if (err == M_ERR || errno)
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
		if (safe_close(redirect_arr[i]->fd))
			return (M_ERR);
		redirect_arr[i]->fd = -1;
		++i;
	}
	return (M_OK);
}

int	pipeline_set_fd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	index;
	int	err;

	index = 0;
	while (pipe_line)
	{
		if (sighandler_set(HEREDOC_MODE))
			return (M_ERR);
		err = make_redirect(minishell, pipe_line->redirect_in, index);
		if (err)
			return (err);
		err = make_redirect(minishell, pipe_line->redirect_out, index);
		if (err)
			return (err);
		if (sighandler_set(DEFAULT_MODE))
			return (M_ERR);
		pipe_line = pipe_line->next;
		++index;
	}
	return (M_OK);
}

int	redirect_pipe(int fd_in, int fd_out)
{
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
	return (M_OK);
}
