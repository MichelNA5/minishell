/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
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

static int	join_with_prev_arg(t_parse_ctx *ctx, char *value)
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
	if (value[0] == '\0' && (current->separated || *ctx->arg_idx == 0))
	{
		free(value);
		return (1);
	}
	if (current->separated == 0 && *ctx->arg_idx > 0)
		return (join_with_prev_arg(ctx, value));
	ctx->parser->cmds[ctx->cmd_idx].args[*ctx->arg_idx] = value;
	(*ctx->arg_idx)++;
	return (1);
}

int	handle_dollar_token(t_parse_ctx *ctx, t_token **cur)
{
	char	*value;
	char	*temp;
	int		separated;

	if (*ctx->arg_idx >= MAX_ARGS - 1)
		return (0);
	separated = (*cur)->separated;
	value = NULL;
	if ((*cur)->next && (*cur)->next->type == WORD)
	{
		temp = ft_strjoin("$", (*cur)->next->value);
		value = expand_env_vars(temp, ctx->shell);
		free(temp);
		*cur = (*cur)->next;
	}
	else
		value = ft_strdup("$");
	if (!value)
		return (0);
	if (value[0] == '\0' && (separated || *ctx->arg_idx == 0))
	{
		free(value);
		return (1);
	}
	if (separated == 0 && *ctx->arg_idx > 0)
		return (join_with_prev_arg(ctx, value));
	ctx->parser->cmds[ctx->cmd_idx].args[*ctx->arg_idx] = value;
	(*ctx->arg_idx)++;
	return (1);
}

t_token	*parse_cmd_tokens(t_parse_ctx *ctx, t_token *current)
{
	int	is_word;
	int	is_redir;

	while (current && current->type != PIPE)
	{
		is_word = (current->type == WORD || current->type == QUOTE_SINGLE
				|| current->type == QUOTE_DOUBLE);
		is_redir = (current->type == REDIR_IN || current->type == REDIR_OUT
				|| current->type == REDIR_APPEND
				|| current->type == REDIR_HEREDOC);
		if (is_word && !handle_word_token(ctx, current))
			return (NULL);
		else if (current->type == DOLLAR
			&& !handle_dollar_token(ctx, &current))
			return (NULL);
		else if (is_redir
			&& !parse_redirection(ctx->parser, ctx->cmd_idx, &current,
				ctx->shell))
			return (NULL);
		current = current->next;
	}
	return (current);
}
