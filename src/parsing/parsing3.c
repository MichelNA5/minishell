/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:45:31 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_cmd_args(t_cmd *cmd)
{
	int	j;

	if (!cmd->args)
		return ;
	j = 0;
	while (cmd->args[j])
	{
		free(cmd->args[j]);
		j++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

static void	free_cmd_redirs(t_cmd *cmd)
{
	int	j;

	if (!cmd->redirs)
		return ;
	j = 0;
	while (j < cmd->redir_count)
	{
		free(cmd->redirs[j].file);
		j++;
	}
	free(cmd->redirs);
	cmd->redirs = NULL;
}

static void	free_cmd(t_cmd *cmd)
{
	free_cmd_args(cmd);
	free_cmd_redirs(cmd);
}

void	free_parser(t_parser *parser)
{
	int	i;

	if (!parser)
		return ;
	i = 0;
	while (i < parser->cmd_count)
	{
		free_cmd(&parser->cmds[i]);
		i++;
	}
	free(parser->cmds);
	free(parser);
}
