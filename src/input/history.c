/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:50:11 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_history(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < MAX_HISTORY_SIZE)
	{
		shell->history[i] = NULL;
		i++;
	}
	shell->history_count = 0;
	shell->history_index = 0;
}

void	add_history_entry(t_shell *shell, char *line)
{
	int	i;

	if (!line)
		return ;
	if (shell->history_count < MAX_HISTORY_SIZE)
	{
		shell->history[shell->history_count] = ft_strdup(line);
		shell->history_count++;
	}
	else
	{
		free(shell->history[0]);
		i = 0;
		while (i < MAX_HISTORY_SIZE - 1)
		{
			shell->history[i] = shell->history[i + 1];
			i++;
		}
		shell->history[MAX_HISTORY_SIZE - 1] = ft_strdup(line);
	}
	shell->history_index = shell->history_count;
}

char	*get_history_command(t_shell *shell, int direction)
{
	if (direction > 0)
	{
		if (shell->history_index > 0)
			shell->history_index--;
	}
	else
	{
		if (shell->history_index < shell->history_count)
			shell->history_index++;
	}
	if (shell->history_index >= 0
		&& shell->history_index < shell->history_count)
		return (shell->history[shell->history_index]);
	return (NULL);
}

void	free_history(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->history_count)
	{
		free(shell->history[i]);
		shell->history[i] = NULL;
		i++;
	}
	shell->history_count = 0;
	shell->history_index = 0;
}
