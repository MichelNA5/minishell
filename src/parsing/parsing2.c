/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/26 00:23:58 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(const char *token);

static int	is_invalid_redir_operand(t_token *operand)
{
	if (!operand)
	{
		print_syntax_error(NULL);
		return (1);
	}
	if (operand->type == PIPE || operand->type == REDIR_IN
		|| operand->type == REDIR_OUT || operand->type == REDIR_APPEND
		|| operand->type == REDIR_HEREDOC)
	{
		print_syntax_error(operand->value);
		return (1);
	}
	return (0);
}

int	parse_commands(t_parser *parser, t_token *tokens, t_shell *shell)
{
	t_token		*current;
	t_parse_ctx	ctx;
	int			cmd_idx;
	int			arg_idx;

	current = tokens;
	cmd_idx = 0;
	while (current && cmd_idx < parser->cmd_count)
	{
		if (!init_cmd(parser, cmd_idx))
			return (0);
		arg_idx = 0;
		ctx.parser = parser;
		ctx.cmd_idx = cmd_idx;
		ctx.arg_idx = &arg_idx;
		ctx.shell = shell;
		current = parse_cmd_tokens(&ctx, current);
		if (!current && cmd_idx < parser->cmd_count - 1)
			return (0);
		parser->cmds[cmd_idx].args[arg_idx] = NULL;
		if (current && current->type == PIPE)
			current = current->next;
		cmd_idx++;
	}
	return (1);
}

int	parse_redirection(t_parser *parser, int cmd_idx, t_token **current,
	t_shell *shell)
{
	t_redir	*redir;
	t_token	*operand;

	if (!current || !*current)
		return (0);
	if (parser->cmds[cmd_idx].redir_count >= MAX_REDIR)
		return (0);
	operand = (*current)->next;
	if (is_invalid_redir_operand(operand))
		return (0);
	redir = &parser->cmds[cmd_idx].redirs[parser->cmds[cmd_idx].redir_count];
	redir->type = (*current)->type;
	redir->file = NULL;
	if (!handle_redir_operand(redir, &operand, shell) || !redir->file)
		return (0);
	parser->cmds[cmd_idx].redir_count++;
	*current = operand;
	return (1);
}
