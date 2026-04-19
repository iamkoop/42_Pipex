CC = cc
CFLAGS = -Werror -Wall -Wextra -g -Ilibft_lib 

FDFFLAGS = -L. -lmlx -lXext -lX11 -lm

CFILES = error_handler.c free.c access.c main.c

OFILES = $(CFILES:.c=.o)


NAME = pipex

HEADER = pipex.h

LIBFT = libft_lib
LIBFT_A = $(LIBFT)/libft.a
CREATE = ar rcs

REMOVE = rm -f


%.o: %.c  $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OFILES)
	make -C $(LIBFT)
	$(CC) $(CFLAGS) $(OFILES) $(LIBFT_A) $(FDFFLAGS) -o $(NAME)

fclean: clean
	$(REMOVE) $(NAME)
	@make -C $(LIBFT) fclean

clean:
	$(REMOVE) $(OFILES)
	@make -C $(LIBFT) clean

re: fclean all

.PHONY: all clean fclean re