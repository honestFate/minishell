/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:56:45 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:56:52 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
