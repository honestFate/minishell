CC = clang
INC = ./includes/
SRC = ./sources/
OBJ = ./obj/
CFLAGS = -Wall -Wextra -g -fPIE
INCLUDES = minishell.h color.h
LIB_DIR = ./libft/
LIB = libft.a

SRCS =	built_in.c env_sort.c env_utils.c env_utils_2.c env.c exec_cmd.c utils.c history.c\
		get_next_line.c get_next_line_utils.c find_cmd.c err_handler.c signal.c heredoc.c

OBJS = $(SRCS:%.c=$(OBJ)%.o)

NAME = minishell

all:	$(NAME)

$(NAME): $(OBJS)
	@make -C $(LIB_DIR)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) -lreadline -ltermcap $(LIB_DIR)$(LIB)

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
