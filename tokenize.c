/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 02:12:38 by naous            ###   ########.fr       */
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

t_token	*tokenize(char *input)
{
	t_token		*head;
	t_token		*current;
	int			i;
	t_token_ctx	ctx;

	head = NULL;
	current = NULL;
	i = 0;
	ctx.input = input;
	ctx.i = &i;
	ctx.len = ft_strlen(input);
	ctx.head = &head;
	ctx.current = &current;
	while (i < ctx.len)
	{
		ctx.separated = skip_spaces(input, &i, ctx.len);
		if (i >= ctx.len)
			break ;
		if (!process_token(&ctx))
			return (NULL);
	}
	return (head);
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
	{
		*ctx->head = new;
		*ctx->current = new;
	}
	else
	{
		(*ctx->current)->next = new;
		*ctx->current = new;
	}
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
