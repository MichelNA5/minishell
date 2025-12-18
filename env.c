/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/18 14:34:54 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_count(char **env)
{
	int	count;

	count = 0;
	if (!env)
		return (0);
	while (env[count])
		count++;
	return (count);
}

char	**copy_env(char **env)
{
	char	**env_copy;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	env_copy = malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env_copy[i] = ft_strdup(env[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

char	*get_env_var(char *name, char **env)
{
	int	i;
	int	len;

	if (!env)
		env = g_env;
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	set_env_var(char *name, char *value)
{
	int		i;
	int		len;
	char	*new_var;
	char	**new_env;
	int		count;

	if (!name || !value)
		return ;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return;
	ft_strlcpy(new_var, name, len);
	ft_strlcat(new_var, "=", len);
	ft_strlcat(new_var, value, len);
	i = 0;
	while (g_env && g_env[i])
	{
		if (ft_strncmp(g_env[i], name, ft_strlen(name)) == 0
			&& g_env[i][ft_strlen(name)] == '=')
		{
			free(g_env[i]);
			g_env[i] = new_var;
			return;
		}
		i++;
	}
	count = env_count(g_env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return ;
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = g_env[i];
		i++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(g_env);
	g_env = new_env;
}

void	unset_env_var(char *name)
{
	int	i;
	int	len;

	if (!name)
		return ;
	len = ft_strlen(name);
	i = 0;
	while (g_env && g_env[i])
	{
		if (ft_strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
		{
			free(g_env[i]);
			while (g_env[i])
			{
				g_env[i] = g_env[i + 1];
				i++;
			}
			return;
		}
		i++;
	}
}
