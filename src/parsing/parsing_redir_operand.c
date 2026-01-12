/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_operand.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 02:12:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_token_value(t_token *token, t_shell *shell)
{
	if (token->type == WORD)
		return (expand_env_vars(token->value, shell));
	else if (token->type == QUOTE_DOUBLE)
		return (expand_env_vars(token->value, shell));
	else if (token->type == QUOTE_SINGLE)
		return (ft_strdup(token->value));
	return (NULL);
}

static int	handle_dollar_operand(t_redir *redir, t_token **operand,
	t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	if ((*operand)->next && (*operand)->next->type == WORD)
	{
		var_name = (*operand)->next->value;
		var_value = get_var_value(var_name, shell);
		redir->file = var_value;
		*operand = (*operand)->next;
	}
	else
		redir->file = ft_strdup("$");
	return (1);
}

static int	concatenate_consecutive_tokens(t_redir *redir, t_token **operand,
	t_shell *shell)
{
	char	*temp;
	char	*next_value;
	t_token	*current;

	current = (*operand)->next;
	while (current && (current->type == WORD || current->type == QUOTE_SINGLE
			|| current->type == QUOTE_DOUBLE) && current->separated == 0)
	{
		next_value = get_token_value(current, shell);
		if (!next_value)
			break ;
		temp = ft_strjoin(redir->file, next_value);
		free(redir->file);
		free(next_value);
		if (!temp)
			return (0);
		redir->file = temp;
		*operand = current;
		current = current->next;
	}
	return (1);
}

int	handle_redir_operand(t_redir *redir, t_token **operand, t_shell *shell)
{
	if ((*operand)->type == DOLLAR)
		return (handle_dollar_operand(redir, operand, shell));
	if (redir->type == REDIR_HEREDOC)
	{
		if ((*operand)->type == QUOTE_SINGLE
			|| (*operand)->type == QUOTE_DOUBLE)
			redir->quoted_delimiter = 1;
		else
			redir->quoted_delimiter = 0;
	}
	redir->file = get_token_value(*operand, shell);
	if (!redir->file)
		return (0);
	return (concatenate_consecutive_tokens(redir, operand, shell));
}
