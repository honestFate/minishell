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

char	*get_word(char *str, char *set)
{
	int		i;
	char	*word;

	i = 0;
	while (str[i])
	{
		if (ft_strchr(set, str[i]))
			break ;
		++i;
	}
	word = malloc((i + 1) * sizeof(char));
	if (!word)
		return (NULL);
	ft_strlcpy(word, str, i + 1);
	return (word);
}

int	heredoc_put_str(t_minishell *minishell, char *line, int fd)
{
	int		i;
	char	*var;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1])
		{
			if (line[i + 1] == '?')
			{
				ft_putnbr_fd(minishell->exit_status, fd);
				i += 2;
			}
			else
			{
				var = get_word(line[i + 1], "$ ");
				if (!var)
					return (M_ERR);
				
			}
		}
		else
			write(fd, line[i], 1);
	}
	
}

int	heredoc(t_minishell *minishell, int expand, char *delimeter)
{
	int		fd;
	char	*line;

	fd = open("/tmp/minishell_heredoc", O_CREAT | O_RDWR, 0644);
	if (fd < 0)
		return (M_ERR);
	while (1)
	{
		line = readline(TERM_CYAN "> " TERM_RESET);
		if (!line && !ft_strcmp(line, delimeter))
			break ;
		if (expand)
			line = expand_var(minishell, line);
		if (!line)
		{
			close(fd);
			return (M_ERR);
		}
	}
	return (fd);
}
