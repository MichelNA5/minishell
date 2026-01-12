/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/24 01:38:24 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*new_quote_token(char *content, char quote)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(content);
		return (NULL);
	}
	if (quote == '\'')
		tok->type = QUOTE_SINGLE;
	else
		tok->type = QUOTE_DOUBLE;
	tok->value = content;
	tok->next = NULL;
	return (tok);
}

t_token	*handle_word(char *input, int *i)
{
	int		start;
	int		len;
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
		return (NULL);
	tok = malloc(sizeof(t_token));
	if (!tok)
	{
		free(word);
		return (NULL);
	}
	tok->type = WORD;
	tok->value = word;
	tok->next = NULL;
	return (tok);
}

t_token	*handle_quotes(char *input, int *i, char quote)
{
	int		start;
	int		j;
	char	*content;

	start = *i + 1;
	j = start;
	while (input[j] && input[j] != quote)
		j++;
	if (!input[j])
	{
		write(STDERR_FILENO, "Error: unclosed quote\n",
			ft_strlen("Error: unclosed quote\n"));
		return (NULL);
	}
	content = ft_substr(input, start, j - start);
	if (!content)
		return (NULL);
	*i = j + 1;
	return (new_quote_token(content, quote));
}
