#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	//char	term_type_buff[TERM_TYPE_BUFF];
	//char	*termtype;
	//int		status;
	char	*line;
	t_minishell	*minishell;
	(void)argc;
	(void)argv;
	(void)envp;
	if (!isatty(STDIN_FILENO))
		return (1);
	/*termtype = getenv("TERM");
	if (!termtype)
		return (1);*/
	/*status = tgetent(term_type_buff, termtype);
	if (status == 0)
		return (1);
	else if (status < 0)
		return (2);*/
	minishell = malloc(sizeof(t_minishell));
	minishell->history_fd = open_history_file(getenv("HOME"));
	ft_read_history(minishell->history_fd);
	while (1)
    {
        line = readline(READLINE_GREEN "$ " READLINE_RESET);
        if (!line)
            break ;
        if (put_history_line(line, minishell->history_fd) < 0)
			printf("history file corupted");
		int error = ft_cd("libft/");
		if (error)
			printf("%s\n", strerror(error));
		errno = 0;
		error = ft_pwd();
		if (error)
			printf("%s\n", strerror(error));
		errno = 0;
		free(line);
    }
	close(minishell->history_fd);
    return (0);
}
