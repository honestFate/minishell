#include "minishell.h"

void	sig_handler(int signal, siginfo_t *info, void *context)
{
	if (signal == SIGINT)
	{

	}
	if (signal == SIGQUIT)
	{

	}
}

int	init_sighandler_default()
{
	struct sigaction	sig_default;
	sigset_t			set;

	ft_memset(&sig_default, 0, sizeof(sig_default));
	sig_default.sa_mask = SA_SIGINFO;
	sig_default.sa_sigaction = &sig_handler;
	set = 0;
	set |= __sigbits(SIGINT);
	set |= __sigbits(SIGQUIT);
	sig_default.sa_mask = set;
	sigaction()
}
