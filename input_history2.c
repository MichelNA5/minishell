/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:49:14 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_left_arrow(char *input, int *pos, char *prompt)
{
	(void)input;
	(void)prompt;
	if (*pos > 0)
	{
		write(STDOUT_FILENO, "\b", 1);
		(*pos)--;
	}
}

void	handle_right_arrow(char *input, int *pos, char *prompt)
{
	int	len;

	(void)prompt;
	len = ft_strlen(input);
	if (*pos < len)
	{
		write(STDOUT_FILENO, &input[*pos], 1);
		(*pos)++;
	}
}

void	handle_backspace(char *input, int *pos, char *prompt)
{
	int	len;

	len = ft_strlen(input);
	if (len > 0 && *pos > 0)
	{
		ft_memmove(&input[*pos - 1], &input[*pos], (len - *pos) + 1);
		(*pos)--;
		refresh_line(input, *pos, prompt);
	}
}

void	handle_char(char *input, int *pos, char c, char *prompt)
{
	int	len;

	len = ft_strlen(input);
	if (len < MAX_INPUT_SIZE - 1)
	{
		ft_memmove(&input[*pos + 1], &input[*pos], (len - *pos) + 1);
		input[*pos] = c;
		(*pos)++;
		refresh_line(input, *pos, prompt);
	}
}
