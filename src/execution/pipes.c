/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:48:26 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipes(t_parser *parser)
{
	int	i;
	int	pipe_fds[2];

	i = 0;
	while (i < parser->cmd_count)
	{
		parser->cmds[i].pipe_in = -1;
		parser->cmds[i].pipe_out = -1;
		i++;
	}
	i = 0;
	while (i < parser->cmd_count - 1)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("pipe");
			return ;
		}
		parser->cmds[i].pipe_out = pipe_fds[1];
		parser->cmds[i + 1].pipe_in = pipe_fds[0];
		i++;
	}
}

void	close_pipes(t_parser *parser)
{
	int	i;

	i = 0;
	while (i < parser->cmd_count)
	{
		if (parser->cmds[i].pipe_in != -1)
			close(parser->cmds[i].pipe_in);
		if (parser->cmds[i].pipe_out != -1)
			close(parser->cmds[i].pipe_out);
		i++;
	}
}

void	execute_pipeline(t_parser *parser, t_shell *shell)
{
	pid_t	pids[1024];
	int		i;

	if (!validate_pipeline(parser, shell))
		return ;
	if (process_heredocs(parser, shell) == -1)
		return ;
	setup_pipes(parser);
	set_signals_for_execution();
	i = 0;
	while (i < parser->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			exec_pipe_child(parser, i, shell);
		else if (pids[i] < 0)
			perror("fork");
		i++;
	}
	close_pipes(parser);
	wait_all_children(parser, shell, pids);
	restore_signals_after_execution();
}
