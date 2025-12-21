/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_non_tty.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:52:26 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_byte(int *out);

static char	*alloc_input(void)
{
	char	*input;

	input = malloc(MAX_INPUT_SIZE);
	if (!input)
		return (NULL);
	ft_memset(input, 0, MAX_INPUT_SIZE);
	return (input);
}

static int	fill_input(char *input)
{
	int	ch;
	int	idx;
	int	status;

	idx = 0;
	while (idx < MAX_INPUT_SIZE - 1)
	{
		status = read_byte(&ch);
		if (status <= 0)
		{
			if (idx == 0)
				return (-1);
			break ;
		}
		if (ch == '\n')
			break ;
		input[idx] = (char)ch;
		idx++;
	}
	return (idx);
}

char	*read_non_tty(void)
{
	char	*input;
	int		idx;

	input = alloc_input();
	if (!input)
		return (NULL);
	idx = fill_input(input);
	if (idx < 0)
	{
		free(input);
		return (NULL);
	}
	input[idx] = '\0';
	return (input);
}
