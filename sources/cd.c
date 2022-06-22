/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:30 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 04:57:11 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_target_path(t_params *data, char *current_path)
{
	char	*env_path;
	char	*target_path;

	target_path = NULL;
	if (!data->list->arg[1])
	{
		env_path = ft_getenv(data->minishell->env_list, "HOME");
		if (!env_path)
			print_error(data->list->arg[0], NO_HOME_VAR, NULL);
		else
			target_path = ft_strdup(env_path);
	}
	else if (ft_strcmp(data->list->arg[1], "-") == 0)
	{
		env_path = ft_getenv(data->minishell->env_list, "OLDPWD");
		if (!env_path)
			print_error(data->list->arg[0], NO_OLDPWD_VAR, NULL);
		else
			target_path = ft_strdup(env_path);
	}
	else if (data->list->arg[1][0] == '/')
		target_path = ft_strdup(data->list->arg[1]);
	else
	{
		ft_strlcat(current_path, "/", PATH_MAX + 2);
		target_path = ft_strjoin(current_path, data->list->arg[1]);
	}
	return (target_path);
}

int	ft_cd(t_params *data)
{
	char	current_path[PATH_MAX + 2];
	char	*target_path;

	if (!getcwd(current_path, PATH_MAX + 1))
		return (BUILTIN_FAIL);
	target_path = create_target_path(data, current_path);
	if (!target_path)
	{
		if (errno)
		{
			print_error(data->list->arg[0], errno, data->list->arg[1]);
			errno = 0;
		}
		return (BUILTIN_FAIL);
	}
	chdir(target_path);
	free(target_path);
	if (!errno)
	{
		envlist_change_val(data->minishell->env_list, "OLDPWD", ft_strdup(current_path));
		if (!getcwd(current_path, PATH_MAX + 1))
			return (BUILTIN_FAIL);
		envlist_change_val(data->minishell->env_list, "PWD", ft_strdup(current_path));
	}
	else
	{
		print_error(data->list->arg[0], errno, data->list->arg[1]);
		errno = 0;
		return (BUILTIN_FAIL);
	}
	return (M_OK);
}
