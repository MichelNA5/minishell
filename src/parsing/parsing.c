/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:27:32 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_parser	*create_parser(int cmd_count)
{
	t_parser	*parser;

	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	parser->cmds = malloc(sizeof(t_cmd) * cmd_count);
	if (!parser->cmds)
	{
		free(parser);
		return (NULL);
	}
	ft_bzero(parser->cmds, sizeof(t_cmd) * cmd_count);
	parser->cmd_count = cmd_count;
	return (parser);
}

t_parser	*parse_tokens(t_token *tokens, t_shell *shell)
{
	t_parser	*parser;
	int			cmd_count;

	if (!validate_tokens_syntax(tokens))
		return (NULL);
	cmd_count = count_commands(tokens);
	parser = create_parser(cmd_count);
	if (!parser)
		return (NULL);
	if (!parse_commands(parser, tokens, shell))
	{
		free_parser(parser);
		return (NULL);
	}
	return (parser);
}
