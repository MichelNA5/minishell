/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_env_var(char *name, char *value)
{
	char	*new_var;
	int		len;

	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return (NULL);
	ft_strlcpy(new_var, name, len);
	ft_strlcat(new_var, "=", len);
	ft_strlcat(new_var, value, len);
	return (new_var);
}

int	update_existing_var(char *name, char *new_var, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			shell->env[i] = new_var;
			return (1);
		}
		i++;
	}
	return (0);
}

int	env_count(char **env)
{
	int	count;

	count = 0;
	if (!env)
		return (0);
	while (env[count])
		count++;
	return (count);
}
