/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by mmakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*do_expansion(char *exp, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	int		var_len;
	char	*result;

	var_name = get_var_name(exp, *i + 1);
	var_value = get_var_value(var_name, shell);
	var_len = ft_strlen(var_name);
	result = replace_env_var(exp, *i, var_len + 1, var_value);
	if (var_value)
		free(var_value);
	free(var_name);
	if (!result)
		return (NULL);
	*i += ft_strlen(var_value);
	return (result);
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
			expanded_input = do_expansion(expanded_input, &i, shell);
			if (!expanded_input)
				return (NULL);
		}
		else
			i++;
	}
	return (expanded_input);
}

char	*replace_env_var(char *input, int start, int len, char *value)
{
	char	*result;
	int		new_len;
	int		i;
	int		j;

	if (!value)
		value = "";
	new_len = ft_strlen(input) - len + ft_strlen(value);
	result = malloc(new_len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < start)
		result[j++] = input[i++];
	i = 0;
	while (value[i])
		result[j++] = value[i++];
	i = start + len;
	while (input[i])
		result[j++] = input[i++];
	result[j] = '\0';
	free(input);
	return (result);
}

char	*get_var_name(char *input, int start)
{
	int		len;
	int		i;
	char	*var_name;

	len = 0;
	i = start;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
	{
		len++;
		i++;
	}
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	i = 0;
	while (i < len)
	{
		var_name[i] = input[start + i];
		i++;
	}
	var_name[i] = '\0';
	return (var_name);
}

char	*get_var_value(char *var_name, t_shell *shell)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
	{
		value = ft_itoa(shell->exit_status);
		return (value);
	}
	value = get_env_var(var_name, shell->env);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
