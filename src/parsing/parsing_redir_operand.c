/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_operand.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 02:12:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_redir_operand(t_redir *redir, t_token **operand, t_shell *shell)
{
	char	*var_name;
	char	*var_value;

	if ((*operand)->type == WORD)
		redir->file = expand_env_vars((*operand)->value, shell);
	else if ((*operand)->type == DOLLAR)
	{
		if ((*operand)->next && (*operand)->next->type == WORD)
		{
			var_name = (*operand)->next->value;
			var_value = get_var_value(var_name, shell);
			redir->file = var_value;
			*operand = (*operand)->next;
		}
		else
			redir->file = ft_strdup("$");
	}
	else if ((*operand)->type == QUOTE_DOUBLE)
		redir->file = expand_env_vars((*operand)->value, shell);
	else if ((*operand)->type == QUOTE_SINGLE)
		redir->file = ft_strdup((*operand)->value);
	else
		return (0);
	return (1);
}
