/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history_arrows.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:49:05 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clear_input_line(char *input, char *prompt)
{
	int	len;

	len = ft_strlen(input);
	write(STDOUT_FILENO, "\r", 1);
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	while (len > 0)
	{
		write(STDOUT_FILENO, " ", 1);
		len--;
	}
	write(STDOUT_FILENO, "\r", 1);
	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
}

void	handle_up_arrow(char *input, int *pos, char *prompt, t_shell *shell)
{
	char	*line;

	line = get_history_command(shell, 1);
	if (line)
	{
		clear_input_line(input, prompt);
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
		*pos = ft_strlen(input);
		write(STDOUT_FILENO, input, ft_strlen(input));
	}
}

void	handle_down_arrow(char *input, int *pos, char *prompt, t_shell *shell)
{
	char	*line;

	line = get_history_command(shell, -1);
	clear_input_line(input, prompt);
	if (line)
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
	else
		ft_memset(input, 0, MAX_INPUT_SIZE);
	*pos = ft_strlen(input);
	write(STDOUT_FILENO, input, ft_strlen(input));
}
