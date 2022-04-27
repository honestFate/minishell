#ifndef MINISHELL_H
# include <term.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <curses.h>
# include <errno.h>
# include "libft.h"
# include "color.h"
# define BUFFSIZE 100
# define TERM_TYPE_BUFF 2048
int	pwd(void);
int	cd(const char *path);
#endif