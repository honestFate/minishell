/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/12 00:37:52 by ndillon           #+#    #+#             */
/*   Updated: 2021/10/12 12:41:19 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	unsigned long	i;
	char			*ptr;

	if (!dstsize)
		return (ft_strlen(src));
	ptr = dst;
	i = 0;
	while (src[i] && i < dstsize - 1)
	{
		ptr[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
