#include "minishell.h"

void	ft_strcpy(char **dst, const char *src)
{
	unsigned long	i;

	i = 0;
	while (src[i])
	{
		*dst = src[i];
		i++;
        ++dst;
	}
}