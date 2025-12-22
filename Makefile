NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c main_helpers.c execution.c execution2.c execution3.c \
	execution_dispatch.c builtins.c builtins2.c builtins_dispatch.c \
	parsing.c parsing2.c parsing3.c parsing4.c parsing_syntax.c \
	parsing_counts.c parsing_init.c \
	tokenize.c token_handlers.c \
	   parsing_helpers.c parsing_redir_operand.c pipes.c redirections.c \
	   redirections_helper.c \
       env.c env_helpers.c signals.c input_history.c input_history2.c \
	history.c input_history_arrows.c input_history_helpers.c \
	input_non_tty.c input_keys.c input_loop.c \
	tab_completion.c tab_completion_helpers.c \
       tab_completion_utils.c env_expansion.c
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
