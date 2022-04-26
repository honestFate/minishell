CC = gcc
INC = ./includes/
SRC = ./sources/
OBJ = ./obj/
C = gcc
CFLAGS = -Wall -Wextra -Werror -g
INCLUDES = minishell.h
LIB_DIR = ./libft/
LIB = libft.a

SRCS =	minishell.c

OBJS = $(addprefix $(OBJ), $(SRCS:%.c=%.o))

NAME = minishell

all:	$(NAME)

$(NAME): $(OBJS)
	@make -C $(LIB_DIR)
	$(CC) -o $(NAME) $(OBJS) $(CFLAGS) -lreadline -ltermcap $(LIB_DIR)$(LIB)

$(OBJ)%.o: $(SRC)%.c $(INC)/$(INCLUDES)
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INC) -I$(LIB_DIR)

libft:
	@make -C libft

clean:	
	rm -rf ${OBJ}

fclean:	clean
	rm -rf $(NAME)

re: clean all

.PHONY:	all clean fclean re