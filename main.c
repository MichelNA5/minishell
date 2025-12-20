/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/19 13:43:52 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_received = 0;
int		g_exit_status = 0;
int		g_cursor_pos = 0;
char	**g_env = NULL;
int		g_should_exit = 0;
char	*g_current_line = NULL;
t_token	*g_current_tokens = NULL;

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	g_env = copy_env(envp);
	setup_signals();
	init_history();
	if (argc > 1)
		display_welcome(argv[1]);
	minishell_loop(envp);
	cleanup_and_exit();
	exit(g_exit_status);
}

void	minishell_loop(char **envp)
{
	char		*line;
	char		*prompt;
	t_token		*tokens;
	t_parser	*parser;
	char		*cwd;
	char		*tmp;

	(void)envp;
	while (1)
	{
		g_should_exit = 0;
		cwd = getcwd(NULL, 0);
		if (!cwd)
			prompt = ft_strdup("minishell$ ");
		else
		{
			tmp = ft_strjoin("minishell:", cwd);
			free(cwd);
			if (!tmp)
				prompt = ft_strdup("minishell$ ");
			else
			{
				prompt = ft_strjoin(tmp, "$ ");
				free(tmp);
				if (!prompt)
					prompt = ft_strdup("minishell$ ");
			}
		}
		line = enhanced_readline(prompt);
		g_current_line = line;
		free(prompt);
		if (!line)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			g_current_line = NULL;
			break ;
		}
		if (ft_strlen(line) > 0)
		{
			add_history_entry(line);
			tokens = tokenize(line);
			g_current_tokens = tokens;
			if (tokens)
			{
				parser = parse_tokens(tokens);
				if (parser)
				{
					execute_commands(parser);
					free_parser(parser);
				}
				free_tokens(tokens);
			}
			g_current_tokens = NULL;
		}
		free(line);
		g_current_line = NULL;
		if (g_should_exit)
			break ;
	}
}

void	cleanup_and_exit(void)
{
	free_history();
	free_array(g_env);
}

void	cleanup_child_process(t_parser *parser)
{
	free_parser(parser);
	if (g_current_tokens)
	{
		free_tokens(g_current_tokens);
		g_current_tokens = NULL;
	}
	if (g_current_line)
	{
		free(g_current_line);
		g_current_line = NULL;
	}
	cleanup_and_exit();
}
