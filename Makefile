NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(SRC_DIR)/main/main.c $(SRC_DIR)/main/main_helpers.c \
	$(SRC_DIR)/execution/execution.c $(SRC_DIR)/execution/execution2.c \
	$(SRC_DIR)/execution/execution2_helpers.c \
	$(SRC_DIR)/execution/execution3_helpers.c \
	$(SRC_DIR)/execution/execution3_helpers2.c $(SRC_DIR)/execution/execution_dispatch.c \
	$(SRC_DIR)/execution/pipes.c \
	$(SRC_DIR)/builtins/builtins.c $(SRC_DIR)/builtins/builtins2.c \
	$(SRC_DIR)/builtins/builtins_dispatch.c \
	$(SRC_DIR)/builtins/builtins_export_helpers.c \
	$(SRC_DIR)/builtins/builtins_exit_helpers.c \
	$(SRC_DIR)/builtins/builtins_pwd_env.c \
	$(SRC_DIR)/parsing/parsing.c $(SRC_DIR)/parsing/parsing2.c \
	$(SRC_DIR)/parsing/parsing3.c $(SRC_DIR)/parsing/parsing4.c \
	$(SRC_DIR)/parsing/parsing_syntax.c $(SRC_DIR)/parsing/parsing_counts.c \
	$(SRC_DIR)/parsing/parsing_init.c $(SRC_DIR)/parsing/parsing_helpers.c \
	$(SRC_DIR)/parsing/parsing_helpers2.c \
	$(SRC_DIR)/parsing/parsing_redir_operand.c \
	$(SRC_DIR)/tokenize/tokenize.c $(SRC_DIR)/tokenize/token_handlers.c \
	$(SRC_DIR)/tokenize/token_utils.c $(SRC_DIR)/tokenize/token_utils2.c \
	$(SRC_DIR)/redirections/redirections_helper.c \
	$(SRC_DIR)/redirections/redirections_heredoc.c \
	$(SRC_DIR)/env/env.c $(SRC_DIR)/env/env2.c $(SRC_DIR)/env/env_helpers.c \
	$(SRC_DIR)/env/env_expansion.c $(SRC_DIR)/env/env_expansion2.c \
	$(SRC_DIR)/signals/signals.c $(SRC_DIR)/signals/signals_heredoc.c \
	$(SRC_DIR)/input/display.c

OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

$(LIBFT_A):
	make -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I. -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
