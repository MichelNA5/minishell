/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by mnaouss           #+#    #+#             */
/*   Updated: 2025/12/22 02:12:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_symbol_token(t_token_ctx *ctx)
{
	if (ctx->input[*ctx->i] == '|')
	{
		add_token(ctx, "|", PIPE);
		(*ctx->i)++;
		return (1);
	}
	if (ctx->input[*ctx->i] == '$')
	{
		add_token(ctx, "$", DOLLAR);
		(*ctx->i)++;
		return (1);
	}
	return (0);
}

static int	handle_quote_token(t_token_ctx *ctx, char quote)
{
	t_token	*qt;

	qt = handle_quotes(ctx->input, ctx->i, quote);
	if (!qt)
	{
		free_tokens(*ctx->head);
		return (0);
	}
	if (quote == '\'')
		add_token(ctx, qt->value, QUOTE_SINGLE);
	else
		add_token(ctx, qt->value, QUOTE_DOUBLE);
	free(qt->value);
	free(qt);
	return (1);
}

static int	handle_word_token_ctx(t_token_ctx *ctx)
{
	t_token	*w;

	w = handle_word(ctx->input, ctx->i);
	if (w)
	{
		add_token(ctx, w->value, WORD);
		free(w->value);
		free(w);
	}
	return (1);
}

static void	handle_redir_tokens(t_token_ctx *ctx)
{
	if (ctx->input[*ctx->i] == '<')
	{
		if (*ctx->i + 1 < ctx->len && ctx->input[*ctx->i + 1] == '<')
		{
			add_token(ctx, "<<", REDIR_HEREDOC);
			*ctx->i += 2;
		}
		else
		{
			add_token(ctx, "<", REDIR_IN);
			(*ctx->i)++;
		}
		return ;
	}
	if (*ctx->i + 1 < ctx->len && ctx->input[*ctx->i + 1] == '>')
	{
		add_token(ctx, ">>", REDIR_APPEND);
		*ctx->i += 2;
		return ;
	}
	add_token(ctx, ">", REDIR_OUT);
	(*ctx->i)++;
}

int	process_token(t_token_ctx *ctx)
{
	if (handle_symbol_token(ctx))
		return (1);
	if (ctx->input[*ctx->i] == '<' || ctx->input[*ctx->i] == '>')
		handle_redir_tokens(ctx);
	else if (ctx->input[*ctx->i] == '\'')
		return (handle_quote_token(ctx, '\''));
	else if (ctx->input[*ctx->i] == '"')
		return (handle_quote_token(ctx, '"'));
	else
		return (handle_word_token_ctx(ctx));
	return (1);
}
