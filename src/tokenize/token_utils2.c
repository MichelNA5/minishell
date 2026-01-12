/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/24 01:38:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirection_token(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == REDIR_HEREDOC);
}

int	update_prev_was_redir(t_token *current)
{
	if (current && is_redirection_token(current->type))
		return (1);
	return (0);
}

static t_token	*execute_tokenize_loop(t_token_ctx *ctx, char *input,
	int *prev_was_redir)
{
	int	ret;

	while (*ctx->i < ctx->len)
	{
		ret = process_token_loop(ctx, input, prev_was_redir);
		if (ret == 0)
			break ;
		if (ret == -1)
			return (NULL);
	}
	return (*ctx->head);
}

void	setup_tokenize_context(t_token_ctx *ctx, char *input,
	t_token_vars *vars)
{
	ctx->input = input;
	ctx->i = vars->i;
	ctx->len = ft_strlen(input);
	ctx->head = vars->head;
	ctx->current = vars->current;
}

t_token	*run_tokenize_loop(t_token_ctx *ctx, char *input, int *prev_was_redir)
{
	return (execute_tokenize_loop(ctx, input, prev_was_redir));
}
