NAME = libft.a

DEP = libft.h Makefile

CFLAGS = -Wall -Wextra -Werror

BONUS = $(wildcard ft_lst*.c)

FILES = $(filter-out $(BONUS), $(wildcard *.c))

OBJ = $(FILES:.c=.o)

BONUS_OBJ = $(BONUS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar -rcs $(NAME) $(OBJ)

bonus: $(BONUS_OBJ)
	@ar -rcs $(NAME) $(BONUS_OBJ)

clean:
	rm -f *.o

fclean: clean
	rm -f libft.a

re: fclean all

.PHONE: all clean fclean re bonus