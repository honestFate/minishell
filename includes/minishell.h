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
# include <sys/types.h>
# include <sys/wait.h>
# include "get_next_line.h"
# include "libft.h"
# include "color.h"
# define BUFFSIZE 100
# define TERM_TYPE_BUFF 2048
# define READ_END 0
# define WRITE_END 1
# define TOO_MANY_ARGS 256
# define USAGE_ERROR 257
# define BIN_ECHO 0
# define BIN_CD 1
# define BIN_PWD 2
# define BIN_EXPORT 3
# define BIN_UNSET 4
# define BIN_ENV 5
# define BIN_EXIT 6
# define BIN_NUM 7
# ifndef PATH_MAX
#  define PATH_MAX 1024
# endif
# define M_OK 0
# define M_ERR 1
# define SIMPLE 0
# define PIPE 1
# define REDIRECT_IN 0
# define REDIRECT_HEREDOC 1
# define REDIRECT_OUT 2
# define REDIRECT_OUT_APPEND 2

//FOR TEST

typedef struct	s_redirect
{
	int		type;
	int		arg1;
	char	*arg2;
	int		fd;
}				t_redirect;

typedef struct	s_pipe_line
{
	char				*cmd;
	char				**argv;
	t_redirect			**redirect_in;
	t_redirect			**redirect_out;
	int					pid;
	struct s_pipe_line	*next;
}				t_pipe_line;
//END

typedef struct	s_env_list
{
	char				*key;
	char				*val;
	struct env_list_s	*next;
}				t_env_list;

typedef struct	s_minishell
{
	t_env_list	*env_list;
	int			env_list_size;
	char		**env_arr;
	char		*history_file;
	int			exit_status;
	int			history_fd;
	int			(*built_in[BIN_NUM])(char **argv, t_minishell *minishell);
}				t_minishell;

//built-in
int	ft_pwd(t_minishell *minishell, char **argv);
int	ft_cd(t_minishell *minishell, char **argv);
int	ft_env(t_minishell *minishell, char **argv);
int	ft_export(t_minishell *minishell, char **argv);
int	ft_echo(t_minishell *minishell, char **argv);
int	ft_unset(t_minishell *minishell, char **argv);
int	ft_exit(t_minishell *minishell, char **argv);

//env
int	env_copy(char ***dst, char **env);
void	env_fill_line(char *line, t_env_list *ptr, int dst_size);
int	env_fill_array(t_minishell *minishell);
int	env_to_array(t_minishell *minishell);
int	env_to_list(t_minishell *minishell, char **envp);
t_env_list	*new_env_elem(char *env_var);
int	env_set_key(t_env_list *elem, char *key);
int	env_set_val(t_env_list *elem, char *val);
void    env_list_clear(t_minishell *shell);
t_env_list	*env_del_elem(t_env_list *env_list);
int	envlist_add_var(t_minishell *minishell, char **argv);
void	env_add_back(t_env_list **env_list, t_env_list *new_elem);
void	shell_sort(char **s, int s_size);
void	set_increment(int *arr);
int		env_change_val(t_env_list *env_list, char *key, char *new_val);

//history
int	open_history_file(char *home_path);
void	ft_read_history(int fd);
int	put_history_line(char *line, int fd);

//utils
void	print_error(char *cmd, int error);
char	*ft_getenv(t_env_list *env_list, char *name);
void	builtin_arr_init(t_minishell *minishell);
int	ft_isspace(char c);
int	check_overflow(char *s);

#endif
