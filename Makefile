CC = clang
INC = ./includes/
SRC = ./sources/
OBJ = ./obj/
CFLAGS = -Wall -Wextra -g -fPIE
INCLUDES = minishell.h color.h
LIB_DIR = ./libft/
LIB = libft.a
RL_LIB = /Users/ndillon/.brew/Cellar/readline/8.1.2/lib/*.a

SRCS =	cd.c			env.c					env_sort.c		env_utils_3.c	exec_command_line.c	\
		find_cmd.c		get_next_line_utils.c	history.c		redirect.c		unset.c				\
		defenv_init.c	env_copy.c				env_utils.c		err_handler.c	exit.c				\
		free_struct.c	heredoc.c				pipe.c			signal.c		utils.c				\
		echo.c			env_list.c				env_utils_2.c	exec_cmd.c		export.c			\
		get_next_line.c	heredoc_utils.c			pwd.c			std_backup.c	utils_2.c			\

OBJS = $(SRCS:%.c=$(OBJ)%.o)

NAME = minishell

all:	$(NAME)

$(NAME): $(OBJS)
	@make -C $(LIB_DIR)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) -ltermcap $(LIB_DIR)$(LIB) -lreadline

obj/%.o: $(SRC)%.c $(INCLUDES:%=$(INC)%)
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC) -I$(LIB_DIR) -I./gnl/

clean:
	@make -C $(LIB_DIR) clean
	rm -rf ${OBJ}

fclean:	clean
	@make -C $(LIB_DIR) fclean
	rm -rf $(NAME)

re: fclean all

.PHONY:	all clean fclean re
