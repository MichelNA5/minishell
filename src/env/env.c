/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/26 00:23:34 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			env_count(char **env);
char		*create_env_var(char *name, char *value);
int			update_existing_var(char *name, char *new_var, t_shell *shell);

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
