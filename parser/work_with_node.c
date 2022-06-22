/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work_with_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtrinida <gtrinida@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 19:20:13 by gtrinida          #+#    #+#             */
/*   Updated: 2022/06/20 07:26:44 by gtrinida         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_list_utils(t_node *tmp, t_rdir *rdir)
{
	if (tmp->n_arg > 0)
		free(tmp->arg);
	while (rdir != NULL)
	{
		tmp->rdir = rdir->next;
		if (rdir->arg)
			free(rdir->arg);
		free(rdir);
		rdir = tmp->rdir;
	}
	free(tmp);
}
void	clear_list(t_node **list)
{
	t_node	*tmp;
	t_rdir	*rdir;
	int		i;

	i = 0;
	tmp = *list;
	while (tmp != NULL)
	{
		*list = tmp->next;
		free(tmp->cmd);
		while (i < tmp->n_arg)
		{
			free(tmp->arg[i]);
			i++;
		}
		i = 0;
		rdir = tmp->rdir;
		clear_list_utils(tmp, rdir);
		tmp = *list;
	}
}

void	print_rdir(t_node *node)
{
	while (node->rdir->next)
	{
		printf("rdir argument: %s\n", node->rdir->arg);
		printf("heredock quot: %d\n", node->rdir->heredock_quote);
		node->rdir = node->rdir->next;
	}
	printf("rdir argument: %s\n", node->rdir->arg);
	printf("heredock quot: %d\n", node->rdir->heredock_quote);
}

void	print_node(t_node *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (node->next)
	{
		i++;
		if (node->rdir)
			print_rdir(node);
		print_node2(node);
		node = node->next;
	}
	if (node->rdir)
		print_rdir(node);
	print_node2(node);
}

void	print_node2(t_node *node)
{
	int	i;

	i = 0;
	printf("command: %s\n", node->cmd);
	while (node->n_arg > i)
	{
		printf("argument: %s\n", node->arg[i]);
		i++;
	}
}
