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

typedef struct	env_list_s
{
	char		*key;
	char		*val;
	env_list_t	*next;
}				env_list_t;

typedef struct	minishell_s
{
	env_list_t	*env_list;
	int			env_list_size;
	char		**env_arr;		
}				minishell_t;

int	pwd(void);
int	cd(const char *path);

env_list_t	*new_env_elem(char *env_var);
int	env_set_key(env_list_t *elem, char *key);
int	env_set_val(env_list_t *elem, char *val);
void    env_list_clear(minishell_t *shell);
void	ft_strcpy(char **dst, const char *src);

#endif