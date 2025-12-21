/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:45:01 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_arg_value(t_parse_ctx *ctx, t_token *current)
{
	if (current->type == QUOTE_SINGLE)
		return (ft_strdup(current->value));
	return (expand_env_vars(current->value, ctx->shell));
}

int	handle_word_token(t_parse_ctx *ctx, t_token *current)
{
	char	*value;

	if (*ctx->arg_idx >= MAX_ARGS - 1)
		return (0);
	if (current->type == WORD && *ctx->arg_idx < MAX_ARGS - 1)
	{
		value = dup_arg_value(ctx, current);
	}
	else if (current->type == QUOTE_SINGLE && *ctx->arg_idx < MAX_ARGS - 1)
	{
		value = dup_arg_value(ctx, current);
	}
	else if (current->type == QUOTE_DOUBLE && *ctx->arg_idx < MAX_ARGS - 1)
	{
		value = dup_arg_value(ctx, current);
	}
	else
		return (1);
	if (!value)
		return (0);
	ctx->parser->cmds[ctx->cmd_idx].args[*ctx->arg_idx] = value;
	(*ctx->arg_idx)++;
	return (1);
}

int	handle_dollar_token(t_parse_ctx *ctx, t_token **cur)
{
	char	*var_name;
	char	*var_value;
	char	*value;

	if (*ctx->arg_idx >= MAX_ARGS - 1)
		return (0);
	if ((*cur)->next && (*cur)->next->type == WORD)
	{
		var_name = (*cur)->next->value;
		var_value = get_var_value(var_name, ctx->shell);
		value = var_value;
		*cur = (*cur)->next;
	}
	else
	{
		value = ft_strdup("$");
	}
	if (!value)
		return (0);
	ctx->parser->cmds[ctx->cmd_idx].args[*ctx->arg_idx] = value;
	(*ctx->arg_idx)++;
	return (1);
}

t_token	*parse_cmd_tokens(t_parse_ctx *ctx, t_token *current)
{
	while (current && current->type != PIPE)
	{
		if (current->type == WORD || current->type == QUOTE_SINGLE
			|| current->type == QUOTE_DOUBLE)
		{
			if (!handle_word_token(ctx, current))
				return (NULL);
		}
		else if (current->type == DOLLAR)
		{
			if (!handle_dollar_token(ctx, &current))
				return (NULL);
		}
		else if (current->type == REDIR_IN || current->type == REDIR_OUT
			|| current->type == REDIR_APPEND || current->type == REDIR_HEREDOC)
		{
			if (!parse_redirection(ctx->parser, ctx->cmd_idx, &current,
					ctx->shell))
				return (NULL);
		}
		current = current->next;
	}
	return (current);
}

int	handle_redir_operand(t_redir *redir, t_token **operand, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	if ((*operand)->type == WORD)
		redir->file = expand_env_vars((*operand)->value, shell);
	else if ((*operand)->type == DOLLAR)
	{
		if ((*operand)->next && (*operand)->next->type == WORD)
		{
			var_name = (*operand)->next->value;
			var_value = get_var_value(var_name, shell);
			redir->file = var_value;
			*operand = (*operand)->next;
		}
		else
			redir->file = ft_strdup("$");
	}
	else if ((*operand)->type == QUOTE_DOUBLE)
		redir->file = expand_env_vars((*operand)->value, shell);
	else if ((*operand)->type == QUOTE_SINGLE)
		redir->file = ft_strdup((*operand)->value);
	else
		return (0);
	return (1);
}
