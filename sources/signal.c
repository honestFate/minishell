#include "minishell.h"

void	sig_default_mode(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	sig_heredoc_mode(int signal)
{
	if (signal == SIGINT)
	{
		//write(1, "\n", 1);
		//rl_on_new_line();
		//rl_replace_line("", 0);
		//rl_redisplay();
		close(STDIN_FILENO);
	}
}

void	sig_exit(int signal)
{
	if (signal == SIGINT)
	{
		exit(100);
	}
}

int	sig_quit(int mode)
{
	struct sigaction	sig_default;

	ft_memset(&sig_default, 0, sizeof(sig_default));
	if (mode == DEFAULT_MODE || mode == HEREDOC_MODE)
		sig_default.sa_handler = SIG_IGN;
	else if (mode == EXEC_MODE)
		sig_default.sa_handler = SIG_DFL;
	else
		return (M_ERR);
	return (sigaction(SIGQUIT, &sig_default, NULL));
}

int	sighandler_set(int mode)
{
	struct sigaction	sig_default;

	if (sig_quit(mode))
		return (M_ERR);
	ft_memset(&sig_default, 0, sizeof(sig_default));
	if (mode == DEFAULT_MODE)
		sig_default.sa_handler = sig_default_mode;
	else if (mode == HEREDOC_MODE)
		sig_default.sa_handler = sig_heredoc_mode;
	else if (mode == EXEC_MODE)
		sig_default.sa_handler = sig_exit;
	else
		return (M_ERR);
	//sigaddset(&set, SIGQUIT);
	//sigaddset(&set, SA_SIGINFO);
	return (sigaction(SIGINT, &sig_default, NULL));
}
