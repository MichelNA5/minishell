/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by mmakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_commands(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 1;
	current = tokens;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

int	count_pipes(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == PIPE)
			count++;
		current = current->next;
	}
	return (count);
}

void	parse_commands(t_parser *parser, t_token *tokens)
{
	t_token	*current;
	int		cmd_idx;
	int		arg_idx;

	current = tokens;
	cmd_idx = 0;
	while (current && cmd_idx < parser->cmd_count)
	{
		parser->cmds[cmd_idx].args = malloc(sizeof(char *) * MAX_ARGS);
		parser->cmds[cmd_idx].redirs = malloc(sizeof(t_redir) * MAX_REDIR);
		parser->cmds[cmd_idx].redir_count = 0;
		parser->cmds[cmd_idx].pipe_in = -1;
		parser->cmds[cmd_idx].pipe_out = -1;
		arg_idx = 0;
		while (current && current->type != PIPE)
		{
			if (current->type == WORD)
			{
				parser->cmds[cmd_idx].args[arg_idx] = expand_env_vars(current->value);
				arg_idx++;
			}
			else if (current->type == QUOTE_SINGLE)
			{
				parser->cmds[cmd_idx].args[arg_idx] = ft_strdup(current->value);
				arg_idx++;
			}
			else if (current->type == QUOTE_DOUBLE)
			{
				parser->cmds[cmd_idx].args[arg_idx] = expand_env_vars(current->value);
				arg_idx++;
			}
			else if (current->type == DOLLAR)
			{
				if (current->next && current->next->type == WORD)
				{
					char	*var_name = current->next->value;
					char	*var_value = get_var_value(var_name);
					parser->cmds[cmd_idx].args[arg_idx] = var_value;
					arg_idx++;
					current = current->next;
				}
				else
				{
					parser->cmds[cmd_idx].args[arg_idx] = ft_strdup("$");
					arg_idx++;
				}
			}
			else if (current->type == REDIR_IN || current->type == REDIR_OUT
				|| current->type == REDIR_APPEND || current->type == REDIR_HEREDOC)
			{
				parse_redirection(parser, &cmd_idx, &current);
			}
			current = current->next;
		}
		parser->cmds[cmd_idx].args[arg_idx] = NULL;
		if (current && current->type == PIPE)
			current = current->next;
		cmd_idx++;
	}
}

void	parse_redirection(t_parser *parser, int *cmd_idx, t_token **current)
{
	t_redir	*redir;

	redir = &parser->cmds[*cmd_idx].redirs[parser->cmds[*cmd_idx].redir_count];
	redir->type = (*current)->type;
	(*current) = (*current)->next;
	if (*current)
	{
		if ((*current)->type == WORD)
		{
			redir->file = expand_env_vars((*current)->value);
			parser->cmds[*cmd_idx].redir_count++;
		}
		else if ((*current)->type == DOLLAR)
		{
			if ((*current)->next && (*current)->next->type == WORD)
			{
				char	*var_name = (*current)->next->value;
				char	*var_value = get_var_value(var_name);
				redir->file = var_value;
				parser->cmds[*cmd_idx].redir_count++;
				(*current) = (*current)->next;
			}
			else
			{
				redir->file = ft_strdup("$");
				parser->cmds[*cmd_idx].redir_count++;
			}
		}
		else if ((*current)->type == QUOTE_DOUBLE)
		{
			redir->file = expand_env_vars((*current)->value);
			parser->cmds[*cmd_idx].redir_count++;
		}
		else if ((*current)->type == QUOTE_SINGLE)
		{
			redir->file = ft_strdup((*current)->value);
			parser->cmds[*cmd_idx].redir_count++;
		}
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
