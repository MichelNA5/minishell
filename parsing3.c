/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by mmakhlou         ###   ########.fr       */
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
		j = 0;
		while (parser->cmds[i].args[j])
		{
			free(parser->cmds[i].args[j]);
			j++;
		}
		free(parser->cmds[i].args);
		j = 0;
		while (j < parser->cmds[i].redir_count)
		{
			free(parser->cmds[i].redirs[j].file);
			j++;
		}
		free(parser->cmds[i].redirs);
		i++;
	}
	free(parser->cmds);
	free(parser);
}
