/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/07 12:39:11 by naous            ###   ########.fr       */
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
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define MAX_ARGS 100
# define MAX_REDIR 10

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
	int				separated;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	int				heredoc_fd;
	int				quoted_delimiter;
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
	char			*current_line;
	t_token			*current_tokens;
}	t_shell;

typedef struct s_parse_ctx
{
	t_parser		*parser;
	int				cmd_idx;
	int				*arg_idx;
	t_shell			*shell;
}	t_parse_ctx;

typedef struct s_token_vars
{
	t_token	**head;
	t_token	**current;
	int		*i;
}	t_token_vars;

typedef struct s_token_ctx
{
	char			*input;
	int				*i;
	int				len;
	int				separated;
	t_token			**head;
	t_token			**current;
}	t_token_ctx;

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
void				add_token(t_token_ctx *ctx, char *value, t_token_type type);
int					update_prev_was_redir(t_token *current);
void				setup_tokenize_context(t_token_ctx *ctx, char *input,
						t_token_vars *vars);
t_token				*run_tokenize_loop(t_token_ctx *ctx, char *input,
						int *prev_was_redir);
int					process_token_loop(t_token_ctx *ctx, char *input,
						int *prev_was_redir);
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
int					handle_word_token(t_parse_ctx *ctx, t_token *current);
int					handle_dollar_token(t_parse_ctx *ctx, t_token **cur);
int					join_with_prev_arg(t_parse_ctx *ctx, char *value);
int					handle_redir_operand(t_redir *redir, t_token **operand,
						t_shell *shell);

/* Execution functions */
void				execute_command(t_cmd *cmd, t_parser *parser,
						t_shell *shell);
void				execute_commands(t_parser *parser, t_shell *shell);
void				execute_pipeline(t_parser *parser, t_shell *shell);
int					validate_pipeline(t_parser *parser, t_shell *shell);
void				exec_pipe_child(t_parser *parser, int i, t_shell *shell);
void				wait_all_children(t_parser *parser, t_shell *shell,
						pid_t *pids);
void				handle_last_child_exit(int status, t_shell *shell);
void				handle_signal_output(int sig, int *newline_printed);
void				execute_external(t_cmd *cmd, t_shell *shell);
int					execute_builtin(t_cmd *cmd, t_shell *shell);
int					is_builtin(char *cmd);
char				*find_executable(char *cmd, t_shell *shell);
int					setup_redirections(t_cmd *cmd, t_shell *shell);
int					handle_heredoc(char *delimiter, t_shell *shell, int quoted);
int					process_heredocs(t_parser *parser, t_shell *shell);
void				restore_redirections(void);
void				restore_fds(int *fds);
int					prepare_command_execution(t_cmd *cmd, t_parser *parser,
						t_shell *shell, int *fds);
void				setup_pipes(t_parser *parser);
void				close_pipes(t_parser *parser);

/* Builtin functions */
int					builtin_echo(t_cmd *cmd, t_shell *shell);
int					builtin_cd(t_cmd *cmd, t_shell *shell);
int					builtin_pwd(t_cmd *cmd, t_shell *shell);
int					builtin_export(t_cmd *cmd, t_shell *shell);
int					builtin_unset(t_cmd *cmd, t_shell *shell);
int					builtin_env(t_cmd *cmd, t_shell *shell);
int					builtin_exit(t_cmd *cmd, t_shell *shell);
void				print_env_vars(t_shell *shell);

/* Builtin helper functions */
int					is_valid_identifier(const char *name);
void				print_export_error(char *arg);
int					process_export_arg(char *arg, t_shell *shell);
int					is_numeric_str(const char *s);
int					parse_ll(const char *s, long long *out);
void				print_exit_numeric_error(char *arg);

/* Environment functions */
char				**copy_env(char **envp);
char				*get_env_var(char *name, char **env);
void				set_env_var(char *name, char *value, t_shell *shell);
void				unset_env_var(char *name, t_shell *shell);
void				free_array(char **array);

/* Signal functions */
void				setup_signals(void);
void				setup_signals_heredoc(void);
void				sigint_handler(int sig);
void				set_signals_for_execution(void);
void				restore_signals_after_execution(void);

/* Input and display functions */
void				display_welcome(char *message);

/* Environment variable expansion */
char				*expand_env_vars(char *input, t_shell *shell);
char				*replace_env_var(char *input, int start, int len,
						char *value);
char				*get_var_name(char *input, int start);
char				*get_var_value(char *var_name, t_shell *shell);

#endif
