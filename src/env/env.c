/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/26 00:23:34 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			env_count(char **env);
char		*create_env_var(char *name, char *value);
int			update_existing_var(char *name, char *new_var, t_shell *shell);

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
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
		return (NULL);
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

static void	add_new_env_var(char *new_var, t_shell *shell)
{
	char	**new_env;
	int		count;
	int		i;

	count = env_count(shell->env);
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return ;
	}
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[count] = new_var;
	new_env[count + 1] = NULL;
	free(shell->env);
	shell->env = new_env;
}

void	set_env_var(char *name, char *value, t_shell *shell)
{
	char	*new_var;

	if (!name || !value)
		return ;
	new_var = create_env_var(name, value);
	if (!new_var)
		return ;
	if (update_existing_var(name, new_var, shell))
		return ;
	add_new_env_var(new_var, shell);
}

void	unset_env_var(char *name, t_shell *shell)
{
	int	i;
	int	len;

	if (!name)
		return ;
	len = ft_strlen(name);
	i = 0;
	while (shell->env && shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
		{
			free(shell->env[i]);
			while (shell->env[i])
			{
				shell->env[i] = shell->env[i + 1];
				i++;
			}
			return ;
		}
		i++;
	}
}
