# include <term.h>
# include <sys/termios.h>
# include <termcap.h>
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

#define READ_END 0
#define WRITE_END 1

typedef struct	s_cmd_list
{
	char	*cmd;
	char	**args;

}				t_cmd_list;

// void	signal_handler(int signum)
// {
// 	//char	*line;
// 	if (signum == SIGQUIT)
// 	{
// 		write(1, "\n", 1);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 		//free(line);
// 	}
// 	signal(SIGQUIT, signal_handler);
// }

char *new_str(char **s)
{
	*s = malloc(30);
	(*s)[0] = 'o';
	(*s)[1] = 'k';
	(*s)[2] = '\0';
	printf("%s\n", *s);
	return(*s);
}

int main(int argc, char *argv[], char *envp[])
{
	(void)argc;

	char	*line;
	line = NULL;
	char *s = new_str(&line);
	write(1, line + 0, 1);
	write(1, line + 1, 1);
	write(1, line + 2, 1);
	printf("%s\n", line);
	printf("%s\n", s);
	// char	*line;

	// while (1)
    // {
    //     line = readline("$minishell ");
    //     if (!line)
    //         break ;
	// 	add_history(line);
	// 	free(line);
	// }
	// rl_clear_history();
	// struct stat buf;
	// int fd = open("TEST", O_CREAT, 0644);
	// fstat(fd, &buf);
	// printf("%d\n", buf.st_mode);
	// printf("%ld\n", buf.st_size);
	// read(0, NULL, 1);
	// struct stat info;
	// if (fstat(fd, &info) != 0)
	// 	printf("ERROR\n");
	// printf("  inode:   %d\n",   (int) info.st_ino);
    //   printf(" dev id:   %d\n",   (int) info.st_dev);
    //   printf("   mode:   %08x\n",       info.st_mode);
    //   printf("  links:   %d\n",         info.st_nlink);
    //   printf("    uid:   %d\n",   (int) info.st_uid);
    //   printf("    gid:   %d\n",   (int) info.st_gid);
	// write(3, "text to fd 3\n", 14);
	// write(4, "text to fd 4\n", 14);
	// write(5, "text to fd 5\n", 14);
	/*errno = 100;
	access("minihell", F_OK);
	printf("%s\n", strerror(errno));*/
	/*int i = 0;
	(void)argc;
	(void)argv;
	char *s = getenv("HOME");
	printf("%s\n", s);
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		++i;
	}
	int fd = open(".", O_TMPFILE | O_RDWR, 0777);
	write(fd, "test", 4);
	char buf[4];
	int t = read(fd, buf, 4);
	printf("%d", t);
	//write(STDOUT_FILENO, buf, 4);
	close(fd);*/
	/*int p[2];

	if (pipe(p) < 0)
	{
		printf("pipe error\n");
		return (1);
	}
	pid_t pid0 = fork();
	if (pid0 < 0)
	{
		printf("fork error\n");
		return (1);
	}
	if (pid0 == 0)
	{
		// char *arg[] = {"ls", NULL};
		char *arg[] = {"cat", "/dev/urandom", NULL};
		//int fd = open("/dev/null", O_CREAT | O_WRONLY);
		dup2(p[WRITE_END], STDOUT_FILENO);
		close(p[READ_END]);
		close(p[WRITE_END]);
		execve("/bin/cat", arg, NULL);
		printf("cat execute failed");
		exit(1);
	}
	else if (pid0 > 0)
	{
		int p2[2];
		if (pipe(p2) < 0)
			exit(1);
		pid_t pid1 = fork();
		if (pid1 < 0)
		{
			printf("fork error\n");
			return (1);
		}
		if (pid1 == 0)
		{
			char *arg[] = {"cat", "-e", NULL};
			//char *arg[] = {"wc", "-l", NULL};
			dup2(p[READ_END], STDIN_FILENO);
			close(p[READ_END]);
			close(p[WRITE_END]);
			dup2(p2[WRITE_END], STDOUT_FILENO);
			close(p2[WRITE_END]);
			close(p2[READ_END]);
			execve("/bin/cat", arg, NULL);
			printf("ls execute failed\n");
			exit(1);
		}
		else if(pid1 > 0)
		{
			close(p[WRITE_END]);
			close(p[READ_END]);
			pid_t pid2 = fork();
			if (pid2 < 0)
			{
				printf("fork error\n");
				return (1);
			}
			if (pid2 == 0)
			{
				// char *arg[] = {"cat", "-e", NULL};
				char *arg[] = {"ls", "-l", NULL};
				dup2(p2[READ_END], STDIN_FILENO);
				close(p2[READ_END]);
				close(p2[WRITE_END]);
				printf("ls execution start\n");
				execve("/usr/bin/ls", arg, NULL);
				printf("ls execute failed\n");
				exit(1);
			}
			else if(pid2 > 0)
			{
				close(p2[WRITE_END]);
				close(p2[READ_END]);
				int	status;
				printf("WAITING...\n");
				waitpid(pid2, &status, WUNTRACED);
				printf("EXEC FINISHED, status - %d\n", status);
				waitpid(pid1, &status, WUNTRACED);
				printf("EXEC FINISHED, status - %d\n", status);
				waitpid(pid0, &status, WUNTRACED);
				printf("EXEC FINISHED, status - %d\n", status);
			}
		}
	}
	*/
	return (0);
}

// void	pipe_test()
// {

// }
