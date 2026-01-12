/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/06 20:32:05 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_arg_value(t_parse_ctx *ctx, t_token *current)
{
	if (current->type == QUOTE_SINGLE)
		return (ft_strdup(current->value));
	return (expand_env_vars(current->value, ctx->shell));
}

int	join_with_prev_arg(t_parse_ctx *ctx, char *value)
{
	char	*joined;
	int		idx;

	idx = *ctx->arg_idx - 1;
	joined = ft_strjoin(ctx->parser->cmds[ctx->cmd_idx].args[idx], value);
	free(ctx->parser->cmds[ctx->cmd_idx].args[idx]);
	free(value);
	if (!joined)
		return (0);
	ctx->parser->cmds[ctx->cmd_idx].args[idx] = joined;
	return (1);
}

static int	is_word_type(t_token_type type)
{
	return (type == WORD || type == QUOTE_SINGLE || type == QUOTE_DOUBLE);
}

static int	handle_empty_value(char *value, t_token *current, t_parse_ctx *ctx)
{
	if (value[0] == '\0' && (current->separated || *ctx->arg_idx == 0))
	{
		free(value);
		return (1);
	}
	return (0);
}

int	handle_word_token(t_parse_ctx *ctx, t_token *current)
{
	char	*value;

	if (*ctx->arg_idx >= MAX_ARGS - 1)
		return (0);
	if (!is_word_type(current->type) || *ctx->arg_idx >= MAX_ARGS - 1)
		return (1);
	value = dup_arg_value(ctx, current);
	if (!value)
		return (0);
	if (handle_empty_value(value, current, ctx))
		return (1);
	if (current->separated == 0 && *ctx->arg_idx > 0)
		return (join_with_prev_arg(ctx, value));
	ctx->parser->cmds[ctx->cmd_idx].args[*ctx->arg_idx] = value;
	(*ctx->arg_idx)++;
	return (1);
}
