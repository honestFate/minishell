/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:30 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:12:31 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_target_path(t_pipe_line *pipe_line, char *current_path)
{
	char	*target_path;

	if (pipe_line->argv[1][0] == '/')
		target_path = ft_strdup(pipe_line->argv[1]);
	else
	{
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, pipe_line->argv[1]);
	}
	return (target_path);
}

int	ft_cd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;

	if (!pipe_line->argv[1])
		return (M_OK);
	if (!getcwd(current_path, PATH_MAX + 1))
		return (M_ERR);
	target_path = create_target_path(pipe_line, current_path);
	if (!target_path)
		return (M_ERR);
	chdir(target_path);
	free(target_path);
	if (!errno)
		envlist_change_val(minishell->env_list, "PWD", target_path);
	else
	{
		print_error(pipe_line->argv[0], errno, pipe_line->argv[1]);
		errno = 0;
		return (M_ERR);
	}
	return (M_OK);
}
