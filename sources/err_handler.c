/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:54 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 05:32:24 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strerr(int error)
{
	if (error <= 102)
		return (strerror(error));
	if (error == INVALID_IDENTIFER)
		return ("not a valid identifier");
	if (error == NOT_NUM_ARG)
		return ("numeric argument required");
	if (error == USAGE_ERROR)
		return ("invalid option");
	if (error == NO_HOME_VAR)
		return ("HOME not set");
	if (error == NO_OLDPWD_VAR)
		return ("OLDPWD not set");
	return ("unknow error");
}

void	print_error(char *cmd, int error, char *arg)
{
	if (error == 0)
		return ;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error == USAGE_ERROR && arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	else if (arg)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(ft_strerr(error), STDERR_FILENO);
}

void	exit_minishell(
	t_minishell *minishell,
	t_pipe_line *pipe_line,
	int exit_status)
{
	ft_putendl_fd("exiting minishell", STDERR_FILENO);
	free_pipe_line(pipe_line);
	free_str_arr(minishell->env_arr);
	env_list_clear(minishell);
	//free(minishell);
	exit(exit_status);
}
