/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/20 13:42:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_parser(t_parser *parser)
{
	int	i;
	int	j;

	if (!parser)
		return;
	i = 0;
	while (i < parser->cmd_count)
	{
		if (parser->cmds[i].args)
		{
			j = 0;
			while (parser->cmds[i].args[j])
			{
				free(parser->cmds[i].args[j]);
				j++;
			}
			free(parser->cmds[i].args);
		}
		j = 0;
		if (parser->cmds[i].redirs)
		{
			while (j < parser->cmds[i].redir_count)
			{
				free(parser->cmds[i].redirs[j].file);
				j++;
			}
			free(parser->cmds[i].redirs);
		}
		i++;
	}
	free(parser->cmds);
	free(parser);
}
