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

int	heredoc_put_str(t_minishell *minishell, char *line, int fd, int expand)
{
	int		i;
	char	*var;
	char	*env_val;

	i = 0;
	while (line[i])
	{
		if (expand && line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
			{
				ft_putnbr_fd(minishell->exit_status, fd);
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

char	*heredoc(t_minishell *minishell, int expand, char *delimeter, int index)
{
	int		fd;
	char	*fname;
	char	*line;

	fname = heredoc_fname(index);
	if (!fname)
		return (NULL);
	fd = open(fname, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(fname);
		return (NULL);
	}
	while (1)
	{
		line = readline(TERM_CYAN "> " TERM_RESET);
		if (!line)
		{
			ft_putendl_fd("minishell: предупреждение: блок here-document разделён знаком конца файла", 1);
			break;
		}
		if (!ft_strcmp(line, delimeter))
		{
			free(line);
			break ;
		}
		if (heredoc_put_str(minishell, line, fd, expand))
		{
			free(fname);
			free(line);
			close(fd);
			return (NULL);
		}
		free(line);
	}
	if (close(fd))
	{
		free(fname);
		return (NULL);
	}
	return (fname);
}
