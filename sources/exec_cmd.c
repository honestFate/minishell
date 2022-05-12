#include "minishell.h"

//TEST
int test(void)
{
    t_pipe_line cmd0 = {.cmd = "ls", .argv = {"-l", NULL}, .redirect_in = , };
}
//END TEST

int exec_pipe_line(t_pipe_line *pipe_line)
{
    if (!pipe_line->next)
}