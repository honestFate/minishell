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

int	is_builtin(char *cmd)
{
	if (!strcmp(cmd, "echo"))
		return (BIN_ECHO);
	if (!strcmp(cmd, "cd"))
		return (BIN_CD);
	if (!strcmp(cmd, "pwd"))
		return (BIN_PWD);
	if (!strcmp(cmd, "export"))
		return (BIN_EXPORT);
	if (!strcmp(cmd, "unset"))
		return (BIN_UNSET);
	if (!strcmp(cmd, "env"))
		return (BIN_ENV);
	if (!strcmp(cmd, "exit"))
		return (BIN_EXIT);
	return (-1);
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

static int	check_overflow_util(char *s, long long int i, int flag)
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

void	safe_free(void *data)
{
	if (data)
		free(data);
}

int safe_close(int fd)
{
	if (fd >= 0)
		return (close(fd));
	return (M_OK);
}

int	list_len(t_pipe_line *data)
{
	int	i;

	i = 0;
	while (data)
	{
		data = data->next;
		++i;
	}
	return (i);
}
