#include "minishell.h"

char	*env_get_val(t_env_list *env_list, char *key)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, key))
			return (env_list->val);
		env_list = env_list->next;
	}
	return (NULL);
}

char	*get_word(char *str)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			break ;
		++i;
	}
	word = ft_substr(str, 0, i);
	return (word);
}

int	expand_var(t_minishell *minishell, char *line, int fd, int i)
{
	char	*var;
	char	*env_val;

	if (line[i + 1] == '?')
	{
		ft_putnbr_fd(g_exit_status, fd);
		i += 2;
	}
	else
	{
		var = get_word(line + i + 1);
		if (!var)
			return (M_ERR);
		env_val = env_get_val(minishell->env_list, var);
		if (env_val)
			write(fd, env_val, ft_strlen(env_val));
		i += ft_strlen(var) + 1;
		free(var);
	}
	return (M_OK);
}

int	heredoc_put_str(t_minishell *minishell, char *line, int fd, int expand)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (expand && line[i] == '$' && line[i + 1])
		{
			if (expand_var(minishell, line, fd, i))
				return (M_ERR);
		}
		else
			write(fd, &line[i++], 1);
	}
	write(fd, "\n", 1);
	return (M_OK);
}

char	*heredoc_fname(int index)
{
	char	*tmp;
	char	*fname;

	tmp = ft_itoa(index);
	if (!tmp)
		return (NULL);
	fname = ft_strjoin("./minishell_heredoc_", tmp);
	free(tmp);
	return (fname);
}

int	heredoc_readline(t_minishell *minishell, t_redirect *r_info, int fd, char *fname)
{
	int		err;
	char	*line;

	while (1)
	{
		err = 1;
		line = readline(TERM_CYAN "> " TERM_RESET);
		if (!isatty(STDIN_FILENO))
			err = M_ERR;	
		else if (!line || !ft_strcmp(line, r_info->arg2))
			err = M_OK;
		else if (heredoc_put_str(minishell, line, fd, r_info->expand))
			err = M_ERR;
		safe_free(line);
		if (err == M_ERR || err == M_OK)
			return (err);
	}
}

char	*heredoc(t_minishell *minishell, t_redirect *r_info, int index)
{
	int		fd;
	char	*fname;
	char	*line;

	fname = heredoc_fname(index);
	if (!fname)
		return (NULL);
	fd = open(fname, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0 || heredoc_readline(minishell, r_info, fd, fname))
	{
		safe_close(fd);
		free(fname);
		fname = NULL;
	}
	return (fname);
}
