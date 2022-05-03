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
	char				*key;
	char				*val;
	struct env_list_s	*next;
}				env_list_t;

typedef struct	minishell_s
{
	env_list_t	*env_list;
	int			env_list_size;
	char		**env_arr;
	char		*history_file;
	int			exit_status;
}				minishell_t;

int	ft_pwd(void);
int	ft_cd(char *path);
int	ft_env(env_list_t *env_list);
//int	ft_export();
//int	ft_echo();
//int	ft_unset();
//int	ft_exit();

int	env_copy(char ***dst, char **env);
void	env_fill_line(char *line, env_list_t *ptr, int dst_size);
int	env_fill_array(minishell_t *minishell);
int	env_to_array(minishell_t *minishell);
int	env_to_list(minishell_t *minishell, char **envp);
env_list_t	*new_env_elem(char *env_var);
int	env_set_key(env_list_t *elem, char *key);
int	env_set_val(env_list_t *elem, char *val);
void    env_list_clear(minishell_t *shell);
env_list_t	*env_del_elem(env_list_t *env_list);
int	envlist_add_var(minishell_t *minishell, char **argv);
void	env_add_back(env_list_t **env_list, env_list_t *new_elem);
void	shell_sort(char **s, int s_size);
void	set_increment(int *arr);

#endif