#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define READ_END 0
#define WRITE_END 1

typedef struct	s_cmd_list
{
	char	*cmd;
	char	**args;

}				t_cmd_list;


int main(int argc, char *argv[], char *envp[])
{
	struct stat buf;
	int fd = open("TEST", O_CREAT, 0644);
	fstat(fd, &buf);
	printf("%d\n", buf.st_mode);
	printf("%ld\n", buf.st_size);
	read(0, NULL, 1);
	struct stat info;
	if (fstat(fd, &info) != 0)
		printf("ERROR\n");
	printf("  inode:   %d\n",   (int) info.st_ino);
      printf(" dev id:   %d\n",   (int) info.st_dev);
      printf("   mode:   %08x\n",       info.st_mode);
      printf("  links:   %d\n",         info.st_nlink);
      printf("    uid:   %d\n",   (int) info.st_uid);
      printf("    gid:   %d\n",   (int) info.st_gid);
	// write(3, "text to fd 3\n", 14);
	// write(4, "text to fd 4\n", 14);
	// write(5, "text to fd 5\n", 14);
	/*errno = 100;
	access("minihell", F_OK);
	printf("%s\n", strerror(errno));*/
	// int i = 0;
	// (void)argc;
	// (void)argv;
	// char *s = getenv("HOME");
	// printf("%s\n", s);
	// while (envp[i])
	// {
	// 	printf("%s\n", envp[i]);
	// 	++i;
	// }
}
	// int p[2];

	// if (pipe(p) < 0)
	// {
	// 	printf("pipe error\n");
	// 	return (1);
	// }
	// pid_t pid0 = fork();
	// if (pid0 < 0)
	// {
	// 	printf("fork error\n");
	// 	return (1);
	// }
	// if (pid0 == 0)
	// {
	// 	// char *arg[] = {"ls", NULL};
	// 	char *arg[] = {"cat", "/dev/urandom", NULL};
	// 	//int fd = open("/dev/null", O_CREAT | O_WRONLY);
	// 	dup2(p[WRITE_END], STDOUT_FILENO);
	// 	close(p[READ_END]);
	// 	close(p[WRITE_END]);
	// 	execve("/bin/cat", arg, NULL);
	// 	printf("cat execute failed");
	// 	exit(1);
	// }
	// else if (pid0 > 0)
	// {
	// 	int p2[2];
	// 	if (pipe(p2) < 0)
	// 		exit(1);
	// 	pid_t pid1 = fork();
	// 	if (pid1 < 0)
	// 	{
	// 		printf("fork error\n");
	// 		return (1);
	// 	}
	// 	if (pid1 == 0)
	// 	{
	// 		char *arg[] = {"cat", "-e", NULL};
	// 		//char *arg[] = {"wc", "-l", NULL};
	// 		dup2(p[READ_END], STDIN_FILENO);
	// 		close(p[READ_END]);
	// 		close(p[WRITE_END]);
	// 		dup2(p2[WRITE_END], STDOUT_FILENO);
	// 		close(p2[WRITE_END]);
	// 		close(p2[READ_END]);
	// 		execve("/bin/cat", arg, NULL);
	// 		printf("ls execute failed\n");
	// 		exit(1);
	// 	}
	// 	else if(pid1 > 0)
	// 	{
	// 		close(p[WRITE_END]);
	// 		close(p[READ_END]);
	// 		pid_t pid2 = fork();
	// 		if (pid2 < 0)
	// 		{
	// 			printf("fork error\n");
	// 			return (1);
	// 		}
	// 		if (pid2 == 0)
	// 		{
	// 			// char *arg[] = {"cat", "-e", NULL};
	// 			char *arg[] = {"echo", "a", NULL};
	// 			dup2(p2[READ_END], STDIN_FILENO);
	// 			close(p2[READ_END]);
	// 			close(p2[WRITE_END]);
	// 			printf("ls execution start\n");
	// 			execve("/bin/echo", arg, NULL);
	// 			printf("ls execute failed\n");
	// 			exit(1);
	// 		}
	// 		else if(pid2 > 0)
	// 		{
	// 			close(p2[WRITE_END]);
	// 			close(p2[READ_END]);
	// 			int	status;
	// 			printf("WAITING...\n");
	// 			waitpid(pid2, &status, WUNTRACED);
	// 			printf("EXEC FINISHED, status - %d\n", status);
	// 			waitpid(pid1, &status, WUNTRACED);
	// 			printf("EXEC FINISHED, status - %d\n", status);
	// 			waitpid(pid0, &status, WUNTRACED);
	// 			printf("EXEC FINISHED, status - %d\n", status);
	// 		}
	// 	}
	// }
	
// 	return (0);
// }

// void	pipe_test()
// {

// }
