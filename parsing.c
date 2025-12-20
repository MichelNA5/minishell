/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/20 13:42:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_token(t_token **head, t_token **current, char *value,
	t_token_type type);

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

static int	validate_tokens_syntax(t_token *tokens)
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
		if (cur->type == PIPE)
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
		}
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT
			|| cur->type == REDIR_APPEND || cur->type == REDIR_HEREDOC)
		{
			if (!cur->next)
			{
				print_syntax_error(NULL);
				return (0);
			}
			if (cur->next->type == PIPE
				|| cur->next->type == REDIR_IN || cur->next->type == REDIR_OUT
				|| cur->next->type == REDIR_APPEND
				|| cur->next->type == REDIR_HEREDOC)
			{
				print_syntax_error(cur->next->value);
				return (0);
			}
		}
		cur = cur->next;
	}
	return (1);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	t_token	*current;
	int		i;
	int		len;

	head = NULL;
	current = NULL;
	i = 0;
	len = ft_strlen(input);
	while (i < len)
	{
		while (i < len && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (i >= len)
			break;
		if (input[i] == '|')
		{
			add_token(&head, &current, "|", PIPE);
			i++;
		}
		else if (input[i] == '<' && i + 1 < len && input[i + 1] == '<')
		{
			add_token(&head, &current, "<<", REDIR_HEREDOC);
			i += 2;
		}
		else if (input[i] == '>')
		{
			if (i + 1 < len && input[i + 1] == '>')
			{
				add_token(&head, &current, ">>", REDIR_APPEND);
				i += 2;
			}
			else
			{
				add_token(&head, &current, ">", REDIR_OUT);
				i++;
			}
		}
		else if (input[i] == '<')
		{
			add_token(&head, &current, "<", REDIR_IN);
			i++;
		}
		else if (input[i] == '$')
		{
			add_token(&head, &current, "$", DOLLAR);
			i++;
		}
		else if (input[i] == '\'')
		{
			t_token *qt = handle_quotes(input, &i, '\'');
			if (!qt)
			{
				free_tokens(head);
				return (NULL);
			}
			add_token(&head, &current, qt->value, QUOTE_SINGLE);
			free(qt->value);
			free(qt);
		}
		else if (input[i] == '"')
		{
			t_token *qt = handle_quotes(input, &i, '"');
			if (!qt)
			{
				free_tokens(head);
				return (NULL);
			}
			add_token(&head, &current, qt->value, QUOTE_DOUBLE);
			free(qt->value);
			free(qt);
		}
		else
		{
			t_token *w = handle_word(input, &i);
			if (w)
			{
				add_token(&head, &current, w->value, WORD);
				free(w->value);
				free(w);
			}
		}
	}
	return (head);
}

void	add_token(t_token **head, t_token **current, char *value,
	t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return;
	new->value = ft_strdup(value);
	new->type = type;
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		*current = new;
	}
	else
	{
		(*current)->next = new;
		*current = new;
	}
}

t_token	*handle_word(char *input, int *i)
{
	int	start;
	int	len;
	char	*word;
	t_token	*tok;

	start = *i;
	len = ft_strlen(input);
	while (*i < len && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '\'' && input[*i] != '"' && input[*i] != '$')
		(*i)++;
	word = ft_substr(input, start, *i - start);
	if (!word)
		return NULL;
	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(word);
		return NULL;
	}
	tok->type = WORD;
	tok->value = word;
	tok->next = NULL;
	return tok;
}

t_token	*handle_quotes(char *input, int *i, char quote)
{
	int	start;
	int	j;
	char	*content;
	t_token	*tok;

	start = *i + 1;
	j = *i + 1;
	while (input[j] && input[j] != quote)
		j++;
	if (!input[j])
	{
		write(STDERR_FILENO, "Error: unclosed quote\n",
			ft_strlen("Error: unclosed quote\n"));
		return NULL;
	}
	content = ft_substr(input, start, j - start);
	if (!content)
		return NULL;
	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(content);
		return NULL;
	}
	tok->type = (quote == '\'') ? QUOTE_SINGLE : QUOTE_DOUBLE;
	tok->value = content;
	tok->next = NULL;
	*i = j + 1;
	return tok;
}

t_parser	*parse_tokens(t_token *tokens)
{
	t_parser	*parser;
	t_token		*current;
	int			cmd_count;

	if (!validate_tokens_syntax(tokens))
		return (NULL);
	parser = malloc(sizeof(t_parser));
	if (!parser)
		return (NULL);
	cmd_count = count_commands(tokens);
	parser->cmds = malloc(sizeof(t_cmd) * cmd_count);
	if (!parser->cmds)
	{
		free(parser);
		return (NULL);
	}
	ft_bzero(parser->cmds, sizeof(t_cmd) * cmd_count);
	parser->cmd_count = cmd_count;
	current = tokens;
	if (!parse_commands(parser, current))
	{
		free_parser(parser);
		return (NULL);
	}
	return (parser);
}
