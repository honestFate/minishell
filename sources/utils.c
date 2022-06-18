#include "minishell.h"

/*
init builtin array for t_minishell struct 
*/
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

/*
return zero if char is space sym, otherwise return >zero 
*/
int	ft_isspace(char c)
{
	if (c == '\t' || c == '\n'
		|| c == '\v' || c == '\f'
		|| c == '\r' || c == ' ')
		return (1);
	return (0);
}

/*
free NULL-terminated str array
*/
void	free_str_arr(char **strings)
{
	int	i;

	i = 0;
	while (strings[i])
		free(strings[i++]);
	free(strings);
}

static int	check_overflow_util(char *s, int i, int flag)
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

/*
check is number in passed string out of LLong int range
*/
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
			return (check_overflow_util(s, i, flag));
		i *= 10;
		++num_len;
		++s;
	}
	return (M_OK);
}
