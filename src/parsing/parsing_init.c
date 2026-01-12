/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:44:14 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_cmd(t_parser *parser, int cmd_idx)
{
	int	i;

	parser->cmds[cmd_idx].args = malloc(sizeof(char *) * MAX_ARGS);
	parser->cmds[cmd_idx].redirs = malloc(sizeof(t_redir) * MAX_REDIR);
	if (!parser->cmds[cmd_idx].args || !parser->cmds[cmd_idx].redirs)
		return (0);
	ft_bzero(parser->cmds[cmd_idx].args, sizeof(char *) * MAX_ARGS);
	ft_bzero(parser->cmds[cmd_idx].redirs, sizeof(t_redir) * MAX_REDIR);
	i = 0;
	while (i < MAX_REDIR)
	{
		parser->cmds[cmd_idx].redirs[i].heredoc_fd = -1;
		i++;
	}
	parser->cmds[cmd_idx].redir_count = 0;
	parser->cmds[cmd_idx].pipe_in = -1;
	parser->cmds[cmd_idx].pipe_out = -1;
	return (1);
}
