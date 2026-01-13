/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/07 12:20:29 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal_received = 0;

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env = copy_env(envp);
	shell.exit_status = 0;
	shell.should_exit = 0;
	shell.current_line = NULL;
	shell.current_tokens = NULL;
	setup_signals();
	if (argc > 1)
		display_welcome(argv[1]);
	minishell_loop(&shell);
	cleanup_and_exit(&shell);
	exit(shell.exit_status);
}

char	*build_prompt(void);

static void	process_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_parser	*parser;

	add_history(line);
	tokens = tokenize(line);
	shell->current_tokens = tokens;
	if (tokens)
	{
		parser = parse_tokens(tokens, shell);
		if (parser)
		{
			execute_commands(parser, shell);
			free_parser(parser);
		}
		else
			shell->exit_status = 2;
		free_tokens(tokens);
	}
	else
		shell->exit_status = 2;
	shell->current_tokens = NULL;
}

static void	handle_signal_received(t_shell *shell)
{
	if (g_signal_received == SIGINT)
	{
		shell->exit_status = 130;
		g_signal_received = 0;
	}
}

static int	process_loop_iteration(t_shell *shell, char *line)
{
	if (!line)
	{
		write(STDOUT_FILENO, "exit\n", 5);
		shell->current_line = NULL;
		return (1);
	}
	if (ft_strlen(line) > 0)
		process_line(line, shell);
	free(line);
	shell->current_line = NULL;
	return (0);
}

void	minishell_loop(t_shell *shell)
{
	char	*line;
	char	*prompt;

	while (1)
	{
		shell->should_exit = 0;
		prompt = build_prompt();
		line = readline(prompt);
		shell->current_line = line;
		free(prompt);
		handle_signal_received(shell);
		if (process_loop_iteration(shell, line))
			break ;
		if (shell->should_exit)
			break ;
	}
}
