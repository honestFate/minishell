/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_flag_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtrinida <gtrinida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:43:44 by gtrinida          #+#    #+#             */
/*   Updated: 2022/06/21 20:14:49 by gtrinida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	checker(const char *key, char *skey)
{
	int	result;

	result = (skey && ft_strlen(key) == ft_strlen(skey)
			&& ft_strncmp(key, skey, ft_strlen(key)) == 0);
	free(skey);
	return (result);
}
//понять что такое kay и skay
char	*ft_get_value(const char *key, char **env)
{
	int		i;
	int		j;
	char	*skey;
	char	*svalue;

	i = -1;
	svalue = 0;
	while (env[++i])
	{
		skey = 0;
		j = 0;
		if (ft_strnstr(env[i], key, ft_strlen(key)))
		{
			while (env[i][j] != '=')
				j++;
			skey = ft_substr(env[i], 0, j);
		}
		if (checker(key, skey))
			svalue = ft_strdup(&env[i][j + 1]);
		if (svalue)
			break ;
	}
	if (!svalue)
		svalue = ft_strdup(" ");
	return (svalue);
}

void	write_one_sym(t_params *data, t_quotes *quot, char *line)
{
	data->line[quot->j] = line[quot->i];
	quot->j++;	
	quot->i++;
}

void	check_flag(t_params *data, t_quotes *quot, char *line)
{
	if (!quot->flag)
	{
		while (line[quot->i] != '$')
		{
			if (line[quot->i] == quot->flag)
				quot->flag = 0;
			write_one_sym(data, quot, line);
		}
	}
	if (quot->flag)
	{
		while (line[quot->i] && quot->flag)
		{
			if (line[quot->i] == '$' && is_nothing(line[quot->i - 1]) && 
			!is_nothing(line[quot->i + 1]))
				return ;
			if (line[quot->i] == quot->flag)
				quot->flag = 0;
			write_one_sym(data, quot, line);
			if (is_nothing(line[quot->i]) && line[quot->i + 1] == '$')
				write_one_sym(data, quot, line);
		}
	}
}