/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/06 20:18:43 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static char	*do_expansion(char *exp, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		var_len;
	int		value_len;
	char	*result;

	var_name = get_var_name(exp, *i + 1);
	var_value = get_var_value(var_name, shell);
	var_len = ft_strlen(var_name);
	value_len = 0;
	if (var_value)
		value_len = ft_strlen(var_value);
	result = replace_env_var(exp, *i, var_len + 1, var_value);
	if (var_value)
		free(var_value);
	free(var_name);
	if (!result)
	{
		free(exp);
		return (NULL);
	}
	*i += value_len;
	return (result);
}

static int	handle_exit_status_var(char **expanded_input, int *i,
	t_shell *shell)
{
	char	*exit_str;
	int		len;

	exit_str = ft_itoa(shell->exit_status);
	*expanded_input = replace_env_var(*expanded_input, *i, 2,
			exit_str);
	if (!*expanded_input)
	{
		free(exit_str);
		return (0);
	}
	len = ft_strlen(exit_str);
	*i += len;
	free(exit_str);
	return (1);
}

static int	process_dollar_var(char **expanded_input, int *i,
	t_shell *shell)
{
	if ((*expanded_input)[*i + 1] == '?')
	{
		if (!handle_exit_status_var(expanded_input, i, shell))
			return (0);
	}
	else if (is_valid_var_start((*expanded_input)[*i + 1]))
	{
		*expanded_input = do_expansion(*expanded_input, i, shell);
		if (!*expanded_input)
			return (0);
	}
	else
		(*i)++;
	return (1);
}

char	*expand_env_vars(char *input, t_shell *shell)
{
	char	*expanded_input;
	int		i;
	int		in_single_quote;

	expanded_input = ft_strdup(input);
	i = 0;
	in_single_quote = 0;
	while (expanded_input && expanded_input[i])
	{
		if (expanded_input[i] == '\'')
			in_single_quote = !in_single_quote;
		if (expanded_input[i] == '$' && !in_single_quote)
		{
			if (!process_dollar_var(&expanded_input, &i, shell))
				return (NULL);
		}
		else
			i++;
	}
	return (expanded_input);
}
