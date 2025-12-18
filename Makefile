NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c execution.c execution2.c builtins.c builtins2.c parsing.c parsing2.c parsing3.c pipes.c redirections.c env.c signals.c input_history.c input_history2.c env_expansion.c
OBJS = $(SRCS:.c=.o)
LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(OBJS) $(LIBFT_A) -o $(NAME)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
