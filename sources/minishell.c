#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	//char	term_type_buff[TERM_TYPE_BUFF];
	//char	*termtype;
	//int		status;
	char	*line;
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
	while (1)
    {
        line = readline(READLINE_GREEN "$ " READLINE_RESET);
        if (!line)
            break ;
        add_history(line);
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
    return (0);
}
