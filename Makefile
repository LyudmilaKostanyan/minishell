NAME = minishell
CC = cc
RDLINE_RESERV = readline-8.1/
RD_DIR = readline
RDLINE_PATH = $(addprefix $(shell pwd)/, $(RD_DIR))
FLAGS = -Wall -Wextra -Werror
f = -fsanitize=address -g
LFLAGS = -lreadline -L$(RD_DIR)/lib -L./libft -lft
IFLAGS = -I./libft -I$(RD_DIR)/include
FILES = $(wildcard *.c)

OBJS = $(FILES:.c=.o)

%.o: %.c Makefile minishell.h
	$(CC) $(FLAGS) $(f) $(IFLAGS) -c $< -o $@

all: lib $(NAME)

$(NAME): $(OBJS)
	$(CC) $(f) $(OBJS) $(LFLAGS) -o $(NAME)

lib:
	make -C libft
	mkdir $(RD_DIR)
	@cd $(RDLINE_RESERV) && exec ./configure --prefix=$(RDLINE_PATH)
	make -C $(RDLINE_RESERV)
	make -C $(RDLINE_RESERV) install

clean:
	make clean -C libft
	rm -f $(OBJS)
	rm -rf $(RD_DIR)
	make -C $(RDLINE_RESERV) distclean

fclean: clean
	make fclean -C libft
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re libs
