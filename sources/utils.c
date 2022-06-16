#include "minishell.h"

void	builtin_arr_init(t_minishell *minishell)
{
	minishell->built_in[BIN_ECHO] = ft_echo;
	minishell->built_in[BIN_CD] = ft_cd;
	minishell->built_in[BIN_PWD] = ft_pwd;
	minishell->built_in[BIN_EXPORT] = ft_export;
	minishell->built_in[BIN_UNSET] = ft_unset;
	minishell->built_in[BIN_ENV] = ft_env;
	minishell->built_in[BIN_EXIT] = ft_exit;
}

int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n'
		|| c == '\v' || c == '\f'
		|| c == '\r' || c == ' ')
		return (1);
	return (0);
}

void	free_str_arr(char **strings)
{
	int	i;

	i = 0;
	while (strings[i])
		free(strings[i++]);
	free(strings);
}

int	check_overflow(char *s)
{
	long long int	i;
	int				num_len;
	int				flag;

	flag = 0;
	while (ft_isspace(*s))
		++s;
	if (*s == '-')
	{
		flag = 1;
		++s;
	}
	num_len = 0;
	while (*s)
	{
		i = i + (*s - 48);
		if (num_len == 17)
		{
			if (!*(s + 1))
				return (M_OK);
			if (*(s + 2))
				return (M_ERR);
			if (i > LLONG_MAX / 10)
				return (M_ERR);
			if (i == LLONG_MAX / 10 && *(s + 1) - 48 > 8 && flag)
				return (M_ERR);
			if (i == LLONG_MAX / 10 && *(s + 1) - 48 > 7 && !flag)
				return (M_ERR);
			return (M_OK);
		}
		i *= 10;
		++num_len;
		++s;
	}
	return (M_OK);
}
