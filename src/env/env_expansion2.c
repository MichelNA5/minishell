/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_expansion2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/06 20:18:43 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
