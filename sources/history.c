#include "minishell.h"

int	open_history_file(char *home_path)
{
	int		fd;
	char	*history_file;

	history_file = ft_strjoin(home_path, "/.minishell_history");
	if (!history_file)
		return (errno);
	printf("%s\n", history_file);
	fd = open((const char *)history_file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	free(history_file);
	printf("%s\n", strerror(errno));
	return (fd);
}

void	ft_read_history(int fd)
{
	char	*line;
	char	*temp;

	while (1)
	{
		temp = get_next_line(fd);
		line = ft_strtrim(temp, "\n");
		free(temp);
		if (!line)
			break ;
		add_history(line);
		free(line);
	}
}

int	put_history_line(char *line, int fd)
{
	int	err;
	add_history(line);
	if (fd <= 0)
		return (0);
	err = write(fd, line, ft_strlen(line));
	if (err < 0)
		return (-1);
	err = write(fd, "\n", 1);
	return (err);
}
