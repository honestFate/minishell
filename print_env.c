#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>

int main(int argc, char **argv, char **envp)
{
// 	int i = 0;
// 	(void)argc;
// 	(void)argv;
// 	char *s = getenv("HOME");
// 	printf("%s\n", s);
// 	while (envp[i])
// 	{
// 		printf("%s\n", envp[i]);
// 		++i;
// 	}
	int p[2];

	if (pipe(p))
	{
		printf("pipe error\n");
		return (1);
	}
	int pid = fork();
	if (pid < 0)
	{
		printf("fork error\n");
		return (1);
	}
	if (pid == 0)
	{
		char **arg = {"ls", NULL};
		close(p[1]);
		dup2(p[0], 1);
		execve("/bin/ls", arg, NULL);
	}
	else if (pid > 0)
	{
		pid = fork();
		if (pid < 0)
		{
			printf("fork error\n");
			return (1);
		}
		if (pid == 0)
		{
			char **arg = {"cat", "-e", NULL};
			close(p[0]);
			dup2(p[1], 0);
			execve("/bin/cat", arg, NULL);
			waitpid(pid, NULL, 0);
		}
		else if(pid > 0)
		{
			int	status;
			printf("WAITING...\n");
			waitpid(pid, &status, WUNTRACED);
			printf("EXEC FINISHED, status - \n", status);
		}
	}
	return (0);
}
