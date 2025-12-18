/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:45:21 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    add_token(t_token **head, t_token **current, char *value,
	t_token_type type);

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
		printf("Error: unclosed quote\n");
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
	parser->cmd_count = cmd_count;
	current = tokens;
	parse_commands(parser, current);
	return (parser);
}
