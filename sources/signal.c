#include "minishell.h"

void	sig_handler(int signal, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

int	sighandler_set(int mode)
{
	struct sigaction	sig_default;
	sigset_t			set;
	
	(void)mode;
	ft_memset(&sig_default, 0, sizeof(sig_default));
	sig_default.sa_sigaction = &sig_handler;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGQUIT);
	sigaddset(&set, SA_SIGINFO);
	sig_default.sa_mask = set;
	sigaction(SIGINT, &sig_default, NULL);
	//sigaction(SIGQUIT, &sig_default, NULL);
	return (0);
}

