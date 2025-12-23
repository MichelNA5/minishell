/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_loop.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:52:26 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_byte(int *out);

static int	handle_input_char(char *input, int ch, char *prompt, t_shell *shell)
{
	if (ch == '\n')
	{
		write(STDOUT_FILENO, "\n", 1);
		return (1);
	}
	handle_special_char(input, ch, prompt, shell);
	return (0);
}

int	process_input_loop(char *input, char *prompt, t_shell *shell)
{
	int	ch;
	int	status;

	while (1)
	{
		status = read_byte(&ch);
		if (status <= 0)
		{
			if (ft_strlen(input) == 0)
				return (-1);
			break ;
		}
		if (handle_input_char(input, ch, prompt, shell))
			break ;
	}
	return (1);
}
