NAME = minishell
CC = cc
FLAGS = -Wall -Wextra -Werror
f = #-fsanitize=address -g
LFLAGS = -lreadline -L./lib -L./libft -lft
IFLAGS = -I./libft -I./include
FILES = $(wildcard *.c)

OBJS = $(FILES:.c=.o)

%.o: %.c Makefile minishell.h
	$(CC) $(FLAGS) $(f) $(IFLAGS) -c $< -o $@

all: lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(f) $(OBJS) $(LFLAGS) -o $(NAME)

lib:
	make -C libft

clear:
	make clean -C libft
	rm -f $(OBJS)

fclean: clear
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re lib