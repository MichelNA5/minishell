/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_syntax.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/26 00:23:44 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_syntax_error(const char *token)
{
	const char	*prefix;

	prefix = "minishell: syntax error near unexpected token `";
	if (!token)
		token = "newline";
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, token, ft_strlen(token));
	write(STDERR_FILENO, "'\n", 2);
}

static int	is_redirection(t_token_type type)
{
	return (type == REDIR_IN || type == REDIR_OUT
		|| type == REDIR_APPEND || type == REDIR_HEREDOC);
}

static int	check_pipe_syntax(t_token *cur)
{
	if (!cur->next)
	{
		print_syntax_error("|");
		return (0);
	}
	if (cur->next->type == PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	return (1);
}

static int	check_redir_syntax(t_token *cur)
{
	if (!cur->next)
	{
		print_syntax_error(NULL);
		return (0);
	}
	if (cur->next->type == PIPE || is_redirection(cur->next->type))
	{
		print_syntax_error(cur->next->value);
		return (0);
	}
	return (1);
}

int	validate_tokens_syntax(t_token *tokens)
{
	t_token	*cur;

	if (!tokens)
		return (0);
	cur = tokens;
	if (cur->type == PIPE)
	{
		print_syntax_error("|");
		return (0);
	}
	while (cur)
	{
		if (cur->type == PIPE && !check_pipe_syntax(cur))
			return (0);
		if (is_redirection(cur->type) && !check_redir_syntax(cur))
			return (0);
		cur = cur->next;
	}
	return (1);
}
