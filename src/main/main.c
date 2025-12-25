/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/25 16:29:39 by naous            ###   ########.fr       */
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
		free_tokens(tokens);
	}
	shell->current_tokens = NULL;
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
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			shell->current_line = NULL;
			break ;
		}
		if (ft_strlen(line) > 0)
			process_line(line, shell);
		free(line);
		shell->current_line = NULL;
		if (shell->should_exit)
			break ;
	}
}
