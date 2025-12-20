/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/20 13:42:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_syntax_error(const char *token)
{
	const char	*prefix;

	prefix = "minishell: syntax error near unexpected token `";
	if (!token)
		token = "newline";
	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, token, ft_strlen(token));
	write(STDERR_FILENO, "'\n", 2);
	g_exit_status = 2;
}

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

int	parse_commands(t_parser *parser, t_token *tokens)
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
		if (!parser->cmds[cmd_idx].args || !parser->cmds[cmd_idx].redirs)
			return (0);
		ft_bzero(parser->cmds[cmd_idx].args, sizeof(char *) * MAX_ARGS);
		ft_bzero(parser->cmds[cmd_idx].redirs, sizeof(t_redir) * MAX_REDIR);
		parser->cmds[cmd_idx].redir_count = 0;
		parser->cmds[cmd_idx].pipe_in = -1;
		parser->cmds[cmd_idx].pipe_out = -1;
		arg_idx = 0;
		while (current && current->type != PIPE)
		{
			if (current->type == WORD)
			{
				if (arg_idx < MAX_ARGS - 1)
					parser->cmds[cmd_idx].args[arg_idx++] = expand_env_vars(current->value);
				if (arg_idx >= MAX_ARGS - 1 || !parser->cmds[cmd_idx].args[arg_idx - 1])
					return (0);
			}
			else if (current->type == QUOTE_SINGLE)
			{
				if (arg_idx < MAX_ARGS - 1)
					parser->cmds[cmd_idx].args[arg_idx++] = ft_strdup(current->value);
				if (arg_idx >= MAX_ARGS - 1 || !parser->cmds[cmd_idx].args[arg_idx - 1])
					return (0);
			}
			else if (current->type == QUOTE_DOUBLE)
			{
				if (arg_idx < MAX_ARGS - 1)
					parser->cmds[cmd_idx].args[arg_idx++] = expand_env_vars(current->value);
				if (arg_idx >= MAX_ARGS - 1 || !parser->cmds[cmd_idx].args[arg_idx - 1])
					return (0);
			}
			else if (current->type == DOLLAR)
			{
				if (current->next && current->next->type == WORD)
				{
					char	*var_name = current->next->value;
					char	*var_value = get_var_value(var_name);
					if (arg_idx < MAX_ARGS - 1)
						parser->cmds[cmd_idx].args[arg_idx++] = var_value;
					if (arg_idx >= MAX_ARGS - 1 || !parser->cmds[cmd_idx].args[arg_idx - 1])
						return (0);
					current = current->next;
				}
				else
				{
					if (arg_idx < MAX_ARGS - 1)
						parser->cmds[cmd_idx].args[arg_idx++] = ft_strdup("$");
					if (arg_idx >= MAX_ARGS - 1 || !parser->cmds[cmd_idx].args[arg_idx - 1])
						return (0);
				}
			}
			else if (current->type == REDIR_IN || current->type == REDIR_OUT
				|| current->type == REDIR_APPEND || current->type == REDIR_HEREDOC)
			{
				if (!parse_redirection(parser, cmd_idx, &current))
					return (0);
			}
			current = current->next;
		}
		parser->cmds[cmd_idx].args[arg_idx] = NULL;
		if (current && current->type == PIPE)
			current = current->next;
		cmd_idx++;
	}
	return (1);
}

int	parse_redirection(t_parser *parser, int cmd_idx, t_token **current)
{
	t_redir	*redir;
	t_token	*operand;

	if (!current || !*current)
		return (0);
	if (parser->cmds[cmd_idx].redir_count >= MAX_REDIR)
		return (0);
	operand = (*current)->next;
	if (!operand)
	{
		print_syntax_error(NULL);
		return (0);
	}
	if (operand->type == PIPE
		|| operand->type == REDIR_IN || operand->type == REDIR_OUT
		|| operand->type == REDIR_APPEND || operand->type == REDIR_HEREDOC)
	{
		print_syntax_error(operand->value);
		return (0);
	}
	redir = &parser->cmds[cmd_idx].redirs[parser->cmds[cmd_idx].redir_count];
	redir->type = (*current)->type;
	redir->file = NULL;
	if (operand)
	{
		if (operand->type == WORD)
		{
			redir->file = expand_env_vars(operand->value);
			parser->cmds[cmd_idx].redir_count++;
		}
		else if (operand->type == DOLLAR)
		{
			if (operand->next && operand->next->type == WORD)
			{
				char	*var_name = operand->next->value;
				char	*var_value = get_var_value(var_name);
				redir->file = var_value;
				parser->cmds[cmd_idx].redir_count++;
				operand = operand->next;
			}
			else
			{
				redir->file = ft_strdup("$");
				parser->cmds[cmd_idx].redir_count++;
			}
		}
		else if (operand->type == QUOTE_DOUBLE)
		{
			redir->file = expand_env_vars(operand->value);
			parser->cmds[cmd_idx].redir_count++;
		}
		else if (operand->type == QUOTE_SINGLE)
		{
			redir->file = ft_strdup(operand->value);
			parser->cmds[cmd_idx].redir_count++;
		}
	}
	if (!redir->file)
		return (0);
	*current = operand;
	return (1);
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
