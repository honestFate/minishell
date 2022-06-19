/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:48 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 06:04:03 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_set_key(t_env_list *elem, char *key)
{
	int		key_len;
	char	*temp;

	key_len = ft_strlen(key) + 1;
	temp = (char *)malloc(key_len * sizeof(char));
	if (!temp)
		return (M_ERR);
	if (elem->key)
		free(elem->key);
	ft_strlcpy(temp, key, key_len);
	elem->key = temp;
	return (M_OK);
}

int	env_set_val(t_env_list *elem, char *val)
{
	if (elem->val)
		free(elem->val);
	elem->val = val;
	return (M_OK);
}

static t_env_list	*new_elem_alloc(char *env_key, char *env_val)
{
	t_env_list	*elem;

	elem = (t_env_list *)malloc(sizeof(t_env_list));
	if (!elem)
		return (NULL);
	elem->next = NULL;
	elem->key = NULL;
	elem->val = NULL;
	if (env_set_key(elem, env_key))
	{
		free(elem);
		elem = NULL;
	}
	else if (env_set_val(elem, env_val))
	{
		free(elem->key);
		free(elem);
		elem = NULL;
	}
	return (elem);
}

t_env_list	*new_env_elem(char *env_var)
{
	int			del_pos;
	char		*env_key;
	char		*env_val;

	del_pos = 0;
	env_val = NULL;
	while (env_var[del_pos] != '=' && env_var[del_pos])
		del_pos++;
	env_key = ft_substr(env_var, 0, del_pos);
	if (!env_key)
		return (NULL);
	if (env_var[del_pos])
	{
		env_val = ft_substr(env_var, del_pos + 1, ft_strlen(env_var));
		if (!env_val)
		{
			free(env_key);
			return (NULL);
		}
	}
	return (new_elem_alloc(env_key, env_val));
}
