/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:48:26 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* execute_commands and execute_pipeline are implemented in execution.c */

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
