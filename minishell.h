/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:54:10 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>

# include <dirent.h>
# include <sys/ioctl.h>
# include "libft/libft.h"

# define MAX_INPUT_SIZE 1024
# define MAX_HISTORY_SIZE 100
# define MAX_ARGS 100
# define MAX_REDIR 10
# define BLUE "\033[0;34m"
# define RESET "\033[0m"

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
	DOLLAR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	int				redir_count;
	int				pipe_in;
	int				pipe_out;
}	t_cmd;

typedef struct s_parser
{
	t_cmd			*cmds;
	int				cmd_count;
}	t_parser;

typedef struct s_shell
{
	char			**env;
	int				exit_status;
	int				should_exit;
	int				cursor_pos;
	char			*current_line;
	t_token			*current_tokens;
	char			*history[MAX_HISTORY_SIZE];
	int				history_count;
	int				history_index;
}	t_shell;

typedef struct s_parse_ctx
{
	t_parser		*parser;
	int				cmd_idx;
	int				*arg_idx;
	t_shell			*shell;
}	t_parse_ctx;

typedef struct s_token_ctx
{
	char			*input;
	int				*i;
	int				len;
	t_token			**head;
	t_token			**current;
}	t_token_ctx;

typedef struct s_tab_ctx
{
	char			*parts[3];
	char			*dir_path;
	char			*common;
	int				matches;
}	t_tab_ctx;

extern int			g_signal_received;

/* Main functions */
int					main(int argc, char **argv, char **envp);
void				minishell_loop(t_shell *shell);
void				cleanup_and_exit(t_shell *shell);
void				cleanup_child_process(t_parser *parser, t_shell *shell);

/* Parsing functions */
t_token				*tokenize(char *input);
t_token				*handle_quotes(char *input, int *i, char quote);
t_token				*handle_word(char *input, int *i);
t_parser			*parse_tokens(t_token *tokens, t_shell *shell);
void				add_token(t_token **head, t_token **current, char *value,
						t_token_type type);
int					process_token(t_token_ctx *ctx);
int					validate_tokens_syntax(t_token *tokens);
int					parse_commands(t_parser *parser, t_token *tokens,
						t_shell *shell);
int					parse_redirection(t_parser *parser, int cmd_idx,
						t_token **current, t_shell *shell);
void				free_parser(t_parser *parser);
void				free_tokens(t_token *tokens);
int					count_commands(t_token *tokens);
int					count_pipes(t_token *tokens);
int					init_cmd(t_parser *parser, int cmd_idx);
t_token				*parse_cmd_tokens(t_parse_ctx *ctx, t_token *current);
int					handle_redir_operand(t_redir *redir, t_token **operand,
						t_shell *shell);

/* Execution functions */
void				execute_command(t_cmd *cmd, t_parser *parser,
						t_shell *shell);
void				execute_commands(t_parser *parser, t_shell *shell);
void				execute_pipeline(t_parser *parser, t_shell *shell);
void				execute_external(t_cmd *cmd, t_shell *shell);
void				execute_builtin(t_cmd *cmd, t_shell *shell);
int					is_builtin(char *cmd);
char				*find_executable(char *cmd, t_shell *shell);
int					setup_redirections(t_cmd *cmd, t_shell *shell);
void				handle_heredoc(char *delimiter, t_shell *shell);
void				restore_redirections(void);
void				setup_pipes(t_parser *parser);
void				close_pipes(t_parser *parser);

/* Builtin functions */
void				builtin_echo(t_cmd *cmd, t_shell *shell);
void				builtin_cd(t_cmd *cmd, t_shell *shell);
void				builtin_pwd(t_cmd *cmd, t_shell *shell);
void				builtin_export(t_cmd *cmd, t_shell *shell);
void				builtin_unset(t_cmd *cmd, t_shell *shell);
void				builtin_env(t_cmd *cmd, t_shell *shell);
void				builtin_exit(t_cmd *cmd, t_shell *shell);
void				print_env_vars(t_shell *shell);

/* Environment functions */
char				**copy_env(char **envp);
char				*get_env_var(char *name, char **env);
void				set_env_var(char *name, char *value, t_shell *shell);
void				unset_env_var(char *name, t_shell *shell);
void				free_array(char **array);

/* Signal functions */
void				setup_signals(void);
void				sigint_handler(int sig);
void				sigquit_handler(int sig);
void				print_error(char *msg);

/* Input and history functions */
void				init_history(t_shell *shell);
char				*enhanced_readline(char *prompt, t_shell *shell);
void				add_history_entry(t_shell *shell, char *line);
char				*get_history_command(t_shell *shell, int direction);
void				free_history(t_shell *shell);
void				display_welcome(char *message);
int					read_byte(int *out);
char				*read_non_tty(void);
int					setup_terminal(struct termios *old, struct termios *new);
void				handle_special_char(char *input, int ch, char *prompt,
						t_shell *shell);
int					process_input_loop(char *input, char *prompt,
						t_shell *shell);
void				handle_up_arrow(char *line, int *pos, char *prompt,
						t_shell *shell);
void				handle_down_arrow(char *line, int *pos, char *prompt,
						t_shell *shell);
void				handle_left_arrow(char *line, int *pos, char *prompt);
void				handle_right_arrow(char *line, int *pos, char *prompt);
void				handle_char(char *line, int *pos, char c, char *prompt);
void				handle_backspace(char *line, int *pos, char *prompt);
void				handle_tab(char *line, int *pos, char *prompt);
void				refresh_line(char *line, int pos, char *prompt);

/* Environment variable expansion */
char				*expand_env_vars(char *input, t_shell *shell);
char				*replace_env_var(char *input, int start, int len,
						char *value);
char				*get_var_name(char *input, int start);
char				*get_var_value(char *var_name, t_shell *shell);

#endif
