/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/24 01:38:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_spaces(char *input, int *i, int len)
{
	int	had_space;

	had_space = 0;
	while (*i < len && (input[*i] == ' ' || input[*i] == '\t'))
	{
		had_space = 1;
		(*i)++;
	}
	return (had_space);
}

int	process_token_loop(t_token_ctx *ctx, char *input, int *prev_was_redir)
{
	ctx->separated = skip_spaces(input, ctx->i, ctx->len);
	if (*ctx->i >= ctx->len)
		return (0);
	if (*prev_was_redir)
		ctx->separated = 1;
	if (!process_token(ctx))
		return (-1);
	*prev_was_redir = update_prev_was_redir(*ctx->current);
	return (1);
}

static void	init_token_vars_and_struct(t_token **head, t_token **current,
	int *i, t_token_vars *vars)
{
	*head = NULL;
	*current = NULL;
	*i = 0;
	vars->head = head;
	vars->current = current;
	vars->i = i;
}

static t_token	*init_and_execute_tokenize(char *input, t_token **head,
	t_token **current, int *i)
{
	t_token_ctx		ctx;
	t_token_vars	vars;
	int				prev_was_redir;

	prev_was_redir = 0;
	init_token_vars_and_struct(head, current, i, &vars);
	setup_tokenize_context(&ctx, input, &vars);
	return (run_tokenize_loop(&ctx, input, &prev_was_redir));
}

t_token	*tokenize(char *input)
{
	t_token		*head;
	t_token		*current;
	int			i;

	return (init_and_execute_tokenize(input, &head, &current, &i));
}
