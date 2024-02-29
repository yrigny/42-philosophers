CC		= cc

CFLAGS		= -Wall -Wextra -Werror -pthread

CFILES		= main.c parse_init.c simulate.c

OFILES		= $(CFILES:.c=.o)

NAME		= philo

all: $(NAME)

$(NAME): $(OFILES)
	$(CC) $(OFILES) -o $(NAME)

$(OFILES): %.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES)
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
