/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/18 14:34:18 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_received = 0;
int		g_exit_status = 0;
int		g_cursor_pos = 0;
char	**g_env = NULL;

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
	return (0);
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
		free(prompt);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strlen(line) > 0)
		{
			add_history_entry(line);
			tokens = tokenize(line);
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
		}
		free(line);
	}
}

void	cleanup_and_exit(void)
{
	free_history();
	free_array(g_env);
}
