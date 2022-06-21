/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:13:20 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 01:29:44 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(t_minishell *minishell, t_pipe_line *pipe_line)
{
	char	path[PATH_MAX + 1];

	(void)minishell;
	if (pipe_line->argv[1])
	{
		print_error(pipe_line->argv[0], USAGE_ERROR,
				pipe_line->argv[1]);
		return (BUILTIN_FAIL);
	}
	if (getcwd(path, PATH_MAX + 1) != NULL)
		printf("%s\n", path);
	else
	{
		print_error(pipe_line->argv[0], errno, NULL);
		errno = 0;
		return (BUILTIN_FAIL);
	}
	return (M_OK);
}
