/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/06 20:32:05 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_dollar_value(t_token **cur, t_shell *shell)
{
	char	*value;
	char	*temp;

	if ((*cur)->next && (*cur)->next->type == WORD)
	{
		temp = ft_strjoin("$", (*cur)->next->value);
		value = expand_env_vars(temp, shell);
		free(temp);
		*cur = (*cur)->next;
	}
	else
		value = ft_strdup("$");
	return (value);
}

int	handle_dollar_token(t_parse_ctx *ctx, t_token **cur)
{
	char	*value;
	int		separated;

	if (*ctx->arg_idx >= MAX_ARGS - 1)
		return (0);
	separated = (*cur)->separated;
	value = expand_dollar_value(cur, ctx->shell);
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
