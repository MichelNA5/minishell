/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/24 01:38:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_token_to_head(t_token_ctx *ctx, t_token *new)
{
	*ctx->head = new;
	*ctx->current = new;
}

static void	add_token_to_tail(t_token_ctx *ctx, t_token *new)
{
	(*ctx->current)->next = new;
	*ctx->current = new;
}

void	add_token(t_token_ctx *ctx, char *value, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = ft_strdup(value);
	new->type = type;
	new->separated = ctx->separated;
	new->next = NULL;
	if (!*ctx->head)
		add_token_to_head(ctx, new);
	else
		add_token_to_tail(ctx, new);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
