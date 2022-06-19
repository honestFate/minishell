/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 02:11:00 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 03:18:51 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include "get_next_line.h"
# include "libft.h"
# include "color.h"
# define BUFFSIZE 100
# define TERM_TYPE_BUFF 2048
# define READ_END 0
# define WRITE_END 1
# define USAGE_ERROR 256
# define INVALID_IDENTIFER 257
# define NOT_NUM_ARG 258
# define BUILTIN_FAIL 259
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
# define PIPE_FIRST 2
# define REDIRECT_IN 0
# define REDIRECT_HEREDOC 1
# define REDIRECT_OUT 2
# define REDIRECT_OUT_APPEND 3
# define DEFAULT_MODE 0
# define HEREDOC_MODE 1
# define EXEC_MODE_PARENT 2
# define EXEC_MODE_CHILD 3
//FOR TEST
volatile int	g_exit_status;
typedef struct s_redirect
{
	int		type;
	int		arg1;
	char	*arg2;
	int		fd;
	char	*fname;
	int		expand;
}				t_redirect;

typedef struct s_pipe_line
{
	char				*cmd;
	char				**argv;
	t_redirect			**redirect_in;
	t_redirect			**redirect_out;
	int					pid;
	struct s_pipe_line	*next;
	struct s_pipe_line	*prev;
}				t_pipe_line;
//END

typedef struct s_env_list
{
	char				*key;
	char				*val;
	struct s_env_list	*next;
}				t_env_list;

typedef struct s_std_backup
{
	int	stdin_backup;
	int	stdout_backup;
	int	stderr_backup;
}				t_std_backup;

typedef struct s_pipe_desc
{
	int	exec_type;
	int	fd_in;
	int	fd_out;
	int	fd_to_close;
}				t_pipe_desc;

typedef struct s_minishell
{
	t_env_list	*env_list;
	int			env_list_size;
	char		**env_arr;
	char		*history_file;
	int			exit_status;
	int			history_fd;
	int			(*built_in[BIN_NUM])(
			struct s_minishell *minishell,
			t_pipe_line *pipe_line);
}				t_minishell;

//built-in
int			ft_pwd(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_cd(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_env(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_export(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_echo(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_unset(t_minishell *minishell, t_pipe_line *pipe_line);
int			ft_exit(t_minishell *minishell, t_pipe_line *pipe_line);

//env
int			env_copy(t_minishell *minishell, char **env);
char		*env_create_line(t_env_list *ptr);
int			env_fill_array(t_minishell *minishell);
int			env_to_array(t_minishell *minishell);
int			env_to_list(t_minishell *minishell, char **envp);
t_env_list	*new_env_elem(char *env_var);
int			env_set_key(t_env_list *elem, char *key);
int			env_set_val(t_env_list *elem, char *val);
void		env_list_clear(t_minishell *shell);
void		env_del_elem(t_env_list *env_list);
void		envlist_delone(t_minishell *minishell, char *key);
int			envlist_add_var(t_minishell *minishell, char **argv);
void		env_add_back(t_env_list **env_list, t_env_list *new_elem);
void		select_sort(t_env_list *ptr);
void		set_increment(int *arr);
int			envlist_change_val(t_env_list *env_list, char *key, char *new_val);
int			envarr_change_val(char **env, char *key, char *val);
int			envvar_validate(char *var);

//find_cmd
int			find_cmd(char *cmd, t_env_list *env_list, char **path_to_cmd);

//exec_command_line
void	get_correct_status(int exit_status);
int	start_exec_pipe(t_minishell *minishell, t_pipe_line *pipe_line);
int	exec_pipe_line(t_minishell *minishell, t_pipe_line *pipe_line);

//exec_cmd
int	exec_cmd(t_minishell *minishell, t_pipe_line *pipe_line,
	int fd_in, int fd_out);
int	exec_in_fork(t_minishell *minishell, t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc);

//pipe
t_pipe_desc	*pipe_desc_init(int exec_type, int fd_in, int fd_out,
	int fd_to_close);
int	pipe_exec(t_minishell *minishell, t_pipe_line *pipe_line,
	t_pipe_desc *pipe_desc);


//history
int			open_history_file(char *home_path);
void		ft_read_history(int fd);
int			put_history_line(char *line, int fd);

//utils
void		builtin_arr_init(t_minishell *minishell);
int			is_builtin(char *cmd);
int			ft_isspace(char c);
int			check_overflow(char *s);
void		free_str_arr(char **strings);
int			safe_close(int fd);
void		safe_free(void *data);
int			list_len(t_pipe_line *data);

//err_handler
char		*ft_strerr(int error);
void		print_error(char *cmd, int error, char *arg);
char		*ft_getenv(t_env_list *env_list, char *name);
void		select_exit_status(int err);
void		exit_minishell(t_minishell *minishell,
				t_pipe_line *pipe_line, int err, char *arg);

//signal
int			sighandler_set(int mode);

//heredoc
char		*heredoc(t_minishell *minishell, t_redirect *r_info, int index);

//heredoc_utils
char		*env_get_val(t_env_list *env_list, char *key);
char		*get_word(char *str);
int			expand_var(t_minishell *minishell, char *line, int fd, int i);
int			heredoc_put_str(t_minishell *minishell, char *line, int fd, int expand);
char		*heredoc_fname(int index);

//redirect
int			make_redirect(t_minishell *minishell,
				t_redirect **redirect, int index);
int			cmd_redirect(t_redirect **redirect_arr);
int			cmd_redirect_close(t_redirect **redirect_arr);
int			pipeline_set_fd(t_minishell *minishell, t_pipe_line *pipe_line);
int			redirect_pipe(int fd_in, int fd_out);

//std_backup
int			stdbackup_copy(t_std_backup *std_backup);
int			stdbackup_set(t_std_backup *std_backup);
int			stdbackup_close(t_std_backup *std_backup);

//defenv_init
int			default_env(t_minishell *minishell);

//free_struct
void		free_pipe_line(t_pipe_line *pipe_line);
void		redirect_clear(t_redirect **redirect_arr);

#endif
