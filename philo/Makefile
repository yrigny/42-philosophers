CC		= cc

CFLAGS		= -Wall -Wextra -Werror -pthread

CFILES		= main.c

CFILES_B	= main_bonus.c pipex_bonus.c

OFILES		= $(CFILES:.c=.o)

OFILES_B	= $(CFILES_B:.c=.o)

NAME		= philo

NAME_B		= philo_bonus

all: $(NAME)

bonus: $(NAME_B)

$(NAME): $(OFILES)
	$(CC) $(OFILES) -o $(NAME)

$(NAME_B): $(OFILES_B)
	$(CC) $(OFILES_B) -o $(NAME_B) 

$(OFILES): %.o: %.c philo.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OFILES_B): %.o: %.c philo_bonus.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OFILES) $(OFILES_B)

fclean: clean
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY : all bonus clean fclean re
