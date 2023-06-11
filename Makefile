NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror
LFLAGS = -lreadline -L./libft -lft
IFLAGS = -I./libft
FILES = main.c
OBJS = $(FILES:.c=.o)

%.o: %.c Makefile minishell.h
	$(CC) $(FLAGS) $(IFLAGS) -c $< -o $@

all: lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(NAME)

lib:
	make -C libft

clear:
	make clean -C libft
	rm -f $(OBJS)

fclean: clear
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libs
