#include "minishell.h"

void	print_tgetnum()
{
	printf("col - %d, lines - %d\n", tgetnum("co"), tgetnum("li"));
	printf("col - %d, lines - %d\n", tgetnum("co"), tgetnum("li"));
	printf("col - %d, lines - %d\n", tgetnum("co"), tgetnum("li"));
}

void	print_tgetflag()
{
	printf("Возможность печатать тильду в терминале - %d(0 - можно)\n", tgetflag("hz"));
}

void	print_tgetstr(char buffer[], char *str)
{
	char *buf = buffer;
	tputs(tgetstr(str, &buffer), 1, ft_putchar);
	ft_bzero(buf, BUFFSIZE);
}

void        padding_tputs(char buf[])
{
	extern char	PC;
	extern short ospeed;
	char        *temp;
	char *buffer;
 
   buffer = buf;
   printf("OSPEED - '%d'\n", ospeed);
   printf("PC - '%d'\n", PC);
   temp = tgetstr("pc", &buffer);
   PC = temp ? *temp : 0;
   if (PC == 0)
       printf("PC не задан\n");
	//free(temp);
   buffer = buf;
   bzero(buffer, 100);
}

int	main(int argc, char **argv, char **env)
{
	char	buffer[BUFFSIZE];
	char	term_type_buff[TERM_TYPE_BUFF];
	char	*termtype;
	int		status;
	(void)argc;
	(void)argv;
	(void)env;
	if (!isatty(STDIN_FILENO))
		return (1);
	termtype = getenv("TERM");
	if (!termtype)
		return (1);
	status = tgetent(term_type_buff, termtype);
	if (status == 0)
		return (1);
	else if (status < 0)
		return (2);
	if (argc >= 2)
	{
		add_history("simple line");
		
		padding_tputs(buffer);
		if (!ft_strcmp(argv[1], "tgetnum"))
			print_tgetnum();
		else if (!ft_strcmp(argv[1], "tgetflag"))
			print_tgetflag();
		else if (!ft_strcmp(argv[1], "tgetstr"))
			print_tgetstr(buffer, argv[2]);
	}
	char *line;
	while (1)
    {
        line = readline(READLINE_GREEN "$ " READLINE_RESET);
        if (!line)
            break ;
        add_history(line);
        puts("wow!");
		int error = cd("libft/");
		if (error)
			printf("%s\n", strerror(error));
		errno = 0;
		error = pwd();
		if (error)
			printf("%s\n", strerror(error));
		errno = 0;
		free(line);
    }
    return (0);
}