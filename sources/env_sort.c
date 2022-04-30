#include "minishell.h"

void	shell_sort(char **s, int s_size)
{
	int		i;
	int		j;
	int		increment;
	char	*temp;

	increment = s_size / 2;
	while (increment > 0)
	{
		i = 0;
		while (s[j])
		{
			j = i;
			temp = s[i];
			while (j >= increment && ft_strcmp(s[j - increment], temp) > 0)
			{
				s[j] = s[j - increment];
				j -= increment;
			}
			s[j] = temp;
		}
		increment -= 2;
		increment /= 2;
	}
}
