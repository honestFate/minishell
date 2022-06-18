/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ndillon <ndillon@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 01:12:59 by ndillon           #+#    #+#             */
/*   Updated: 2022/06/19 02:09:27 by ndillon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	get_correct_status(int exit_status)
{
	if (exit_status >= 0)
	{
		if (WIFEXITED(exit_status) && !WEXITSTATUS(exit_status))
			g_exit_status = 0;
		else
		{
			if (WIFSIGNALED(exit_status))
				g_exit_status = WTERMSIG(exit_status) + 128;
			else
				g_exit_status = WEXITSTATUS(exit_status);
		}
	}
}

int	start_exec_pipe(t_minishell *minishell, t_pipe_line *pipe_line)
{
	int	exit_status;

	if (pipe_line->next)
		pipe_exec(minishell, pipe_line, pipe_desc_init(PIPE, -1, -1, -1));
	else
		pipe_exec(minishell, pipe_line, pipe_desc_init(SIMPLE, -1, -1, -1));
	while (pipe_line)
	{
		if (pipe_line->pid > 0)
		{
			if (pipe_line->next)
			{
				if (waitpid(pipe_line->pid, NULL, WUNTRACED) < 0)
					return (M_ERR);
			}
			else if (waitpid(pipe_line->pid, &exit_status, WUNTRACED))
				return (M_ERR);
		}
		pipe_line = pipe_line->next;
	}
	return (exit_status);
}

int	exec_pipe_line(t_minishell *minishell, t_pipe_line *pipe_line)
{
	t_std_backup	std_backup;
	t_pipe_line		*pipe_line;
	int				exit_status;

	exit_status = -1;
	if (stdbackup_copy(&std_backup))
		fatal_err(minishell, pipe_line, errno);
	if (pipeline_set_fd(minishell, pipe_line))
	{
		if (isatty(STDIN_FILENO) || stdbackup_set(&std_backup))
			fatal_err(minishell, pipe_line, errno);
	}
	if (!pipe_line->next && is_builtin(pipe_line->cmd) >= 0)
		g_exit_status = exec_cmd(minishell, pipe_line, -1, -1);
	else if (start_exec_pipe(minishell, pipe_line))
		fatal_err(minishell, pipe_line, errno);
	if (stdbackup_set(&std_backup) || stdbackup_close(&std_backup))
		fatal_err(minishell, pipe_line, errno);
	if (sighandler_set(DEFAULT_MODE))
		fatal_err(minishell, pipe_line, errno);
	get_correct_status(exit_status);
	return (g_exit_status);
}

//TEST
int	main(int argc, char *argv[], char *envp[])
{
	(void)argc;
	(void)argv;
	rl_catch_signals = 0;
	g_exit_status = 0;
	t_minishell minishell = {.exit_status = 0};
	env_copy(&minishell, envp);
	if (env_to_list(&minishell, envp))
	{
		printf("ENV TO LIST FAILED %s\n", NULL);
		exit(2);
	}
	default_env(&minishell);
	builtin_arr_init(&minishell);
	char *cmd_line;
	char *cmd_argv;
	char *redirect_in;
	char *redirect_out;
	char *is_pipe;
	t_pipe_line *pipe_line = NULL;
	t_pipe_line *temp;
	int i = 0;
	sighandler_set(DEFAULT_MODE);
	//sighandler_set(HEREDOC_MODE);
	//sighandler_set(DEFAULT_MODE);
	while (1)
	{
		is_pipe = (char *)1;
		while (is_pipe)
		{
			i = 0;
			cmd_line = readline("> ");
			if (!cmd_line)
			{
				printf("exit\n");
				return (M_OK);
			}
			cmd_argv = readline("argv> ");
			redirect_in = readline("redirect in [from to type] ");
			redirect_out = readline("redirect out [from to type] ");
			is_pipe = readline("is pipe?");
			temp = (t_pipe_line *)malloc(sizeof(t_pipe_line));
			temp->cmd = cmd_line;
			if (*cmd_argv)
			{
				char *temp_argv = ft_strjoin(cmd_line, " ");
				char *temp_argv_1 = ft_strjoin(temp_argv, cmd_argv);
				safe_free(temp_argv);
				temp->argv = ft_split(temp_argv_1, ' ');
				safe_free(temp_argv_1);
			}
			else
			{
				temp->argv = ft_split(cmd_line, ' ');
			}
			write(1, "ok\n", 3);
			if (*redirect_in)
				temp->redirect_in = (t_redirect **)malloc(sizeof(t_redirect *) * 2);
			else
				temp->redirect_in = NULL;
			if (*redirect_out)
				temp->redirect_out = (t_redirect **)malloc(sizeof(t_redirect *) * 2);
			else
				temp->redirect_out = NULL;
			if (*redirect_in)
			{
				write(1, "ok\n", 3);
				char **redirect_in_splited = ft_split(redirect_in, ' ');
				temp->redirect_in[i] = (t_redirect *)malloc(sizeof(t_redirect));
				temp->redirect_in[i]->arg1 = ft_atoi(redirect_in_splited[1]);
				temp->redirect_in[i]->arg2 = ft_strdup(redirect_in_splited[0]);
				temp->redirect_in[i]->type = ft_atoi(redirect_in_splited[2]);
				temp->redirect_in[i]->fname = NULL;
				i++;
				temp->redirect_in[i] = NULL;
				free_str_arr(redirect_in_splited);
			}
			i = 0;
			if (*redirect_out)
			{
				write(1, "ok\n", 3);
				char **redirect_out_splited = ft_split(redirect_out, ' ');
				temp->redirect_out[i] = (t_redirect *)malloc(sizeof(t_redirect));
				temp->redirect_out[i]->arg1 = ft_atoi(redirect_out_splited[0]);
				temp->redirect_out[i]->arg2 = ft_strdup(redirect_out_splited[1]);
				temp->redirect_out[i]->type = ft_atoi(redirect_out_splited[2]);
				temp->redirect_out[i]->fname = NULL;
				i++;
				temp->redirect_out[i] = NULL;
				free_str_arr(redirect_out_splited);
			}
			if (!pipe_line)
			{
				pipe_line = temp;
				pipe_line->prev = NULL;
			}
			else
			{
				temp->prev = pipe_line;
				pipe_line->next = temp;
				pipe_line = pipe_line->next;
			}
			pipe_line->next = NULL;
			if (!*is_pipe)
				break;
		}
		write(1, "ok\n", 3);
		while (pipe_line->prev)
			pipe_line = pipe_line->prev;
		int exit_status = exec_pipe_line(&minishell, pipe_line);
		write(1, "free start\n", 11);
		free_pipe_line(pipe_line);
		write(1, "free end\n", 9);
		pipe_line = NULL;
		//safe_free(cmd_line);
		//safe_free(redirect_in);
		//safe_free(redirect_out);
		safe_free(is_pipe);
		printf("exit status is %d\n", exit_status);
	}
}
//END TEST
