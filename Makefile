NAME = minishell

#
RD_DIR = readline
RDLINE_RESERV = readline-8.1/
RDLINE_PATH = $(addprefix $(shell pwd)/, $(RD_DIR))

#
IFLAGS = -I./libft
LFLAGS = -lreadline -L./libft -lft

#
PREF_SRC = src
PREF_OBJ = obj

#
SRCS = $(addprefix $(PREF_SRC)/, builtins.c errors.c export.c\
	main.c path.c quotes.c read_input.c\
	redirection.c rm_quotes.c utils.c)
OBJS = $(patsubst $(PREF_SRC)/%.c, $(PREF_OBJ)/%.o, $(SRCS))

#
CC = cc
FLAGS = -Wall -Wextra -Werror
FSANITIZE = -fsanitize=address -g

#
all: readline $(NAME)

#
$(NAME): $(OBJS)
	@$(CC) $(FSANITIZE) $(OBJS) $(LFLAGS) -o $(NAME)

#
$(PREF_OBJ)/%.o: $(PREF_SRC)/%.c Makefile src/minishell.h
	@mkdir -p $(PREF_OBJ)
	@$(CC) $(FLAGS) $(FSANITIZE) $(IFLAGS) -c $< -o $@

#
readline: Makefile
	@if [ ! -d $(RD_DIR) ]; then \
		make lib; \
	fi

#
lib:
	@make -C libft
	# @mkdir $(RD_DIR)
	# @cd $(RDLINE_RESERV) && exec ./configure --prefix=$(RDLINE_PATH)
	# @make -C $(RDLINE_RESERV)
	# @make -C $(RDLINE_RESERV) install

#
clean:
	@make clean -C libft
	@rm -rf $(PREF_OBJ)
	@rm -rf $(RD_DIR)
	# @make -C $(RDLINE_RESERV) distclean

#
fclean: clean
	make fclean -C libft
	rm -f $(NAME)

#
re: fclean all

#
.PHONY: all clean fclean re lib readline