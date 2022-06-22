/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparing_for_the_structure.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtrinida <gtrinida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:43:44 by gtrinida          #+#    #+#             */
/*   Updated: 2022/06/21 20:14:27 by gtrinida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	data_node_init(t_params *data)
{
	data->node->n_rdir_arg = 0;
	data->node->need_to_assign = 0;
	data->node->next = NULL;
	data->node->prev = NULL;
	data->node->rdir = NULL;
	data->node->arg = NULL;
}

void	change_quotes(t_params *data, char flag, int i)
{
	while (data->line[i])
	{
		if (data->line[i] == '\"' && !flag)
		{
			flag = data->line[i];
			data->line[i] = '\r';
		}
		if (data->line[i] == '\'' && !flag)
		{
			check_dollar_with_single(data, i);
			flag = data->line[i];
			data->line[i] = '\r';
		}
		else if (data->line[i] == flag && flag)
		{
			flag = 0;
			data->line[i] = '\r';
		}
		i++;
	}
}

void	last_preparation_utils(t_params *data, t_quotes *quot, int i)
{
	i = 1;
	while (i <= data->node->n_arg)
	{	
		if (find_arguments(data, quot))
		{	
			fill_argument(data, quot);
			quot->i_arg++;
		}
		if (data->node->need_to_assign)
		{
			start_work_rdir(data, quot);
			data->node->need_to_assign = 0;
		}
		i++;
	}
	if (data->node->n_arg != 0)
	{
		if (data->node->n_arg == 1 && data->node->n_rdir_arg == 1)
			data->node->n_arg = 1;
		else
			data->node->n_arg -= data->node->n_rdir_arg;
	}
	quot->i_arg = 0;
	add_node(&(data->list), data->node);
}

void	last_preparation(t_params *data, t_quotes *quot, int i, int flag)
{
	if (quot->j == 0)
		return ;
	while (is_nothing(data->line[i]))
		i++;
	if (!data->key)
			i += 2;
	printf("dl is %s\n", data->line);
	change_quotes(data, flag, i);
	check_dollar_with_quest(data);
	data->node = malloc(sizeof(t_node));
	data_node_init(data);
	find_command(data, quot);
	fill_command(data, quot);
	if (data->node->need_to_assign)
	{	
		start_work_rdir(data, quot);
		data->node->need_to_assign = 0;
	}
	data->node->n_arg = argument_count(data, quot, 0, 0);
	if (data->node->n_arg != 0)
		data->node->arg = malloc(sizeof(char *) * data->node->n_arg);
	last_preparation_utils(data, quot, i);
}
