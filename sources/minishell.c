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
	char        *temp;
	char *buffer;
 
   buffer = buf;
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
		padding_tputs(buffer);
		if (!ft_strcmp(argv[1], "tgetnum"))
			print_tgetnum();
		else if (!ft_strcmp(argv[1], "tgetflag"))
			print_tgetflag();
		else if (!ft_strcmp(argv[1], "tgetstr"))
			print_tgetstr(buffer, argv[2]);
	}
	char *s = readline(">");
	printf("%s\n", s);
	free(s);
    return (0);
}