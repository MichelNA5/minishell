/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:47:57 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_cursor_pos;
char		*get_history_command(int direction);

void	handle_up_arrow(char *input, int *pos, char *prompt)
{
	char	*line;
	int		len;

	line = get_history_command(1);
	if (line)
	{
		len = ft_strlen(input);
		printf("\r%s", prompt);
		while (len > 0)
		{
			printf(" ");
			len--;
		}
		printf("\r%s", prompt);
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
		*pos = ft_strlen(input);
		printf("%s", input);
		fflush(stdout);
	}
}

void	handle_down_arrow(char *input, int *pos, char *prompt)
{
	char	*line;
	int		len;

	line = get_history_command(-1);
	if (line)
	{
		len = ft_strlen(input);
		printf("\r%s", prompt);
		while (len > 0)
		{
			printf(" ");
			len--;
		}
		printf("\r%s", prompt);
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
		*pos = ft_strlen(input);
		printf("%s", input);
		fflush(stdout);
	}
	else
	{
		len = ft_strlen(input);
		printf("\r%s", prompt);
		while (len > 0)
		{
			printf(" ");
			len--;
		}
		printf("\r%s", prompt);
		ft_memset(input, 0, MAX_INPUT_SIZE);
		*pos = 0;
	}
}

void	handle_left_arrow(char *input, int *pos, char *prompt)
{
	(void)input;
	(void)prompt;
	if (*pos > 0)
	{
		printf("\b");
		(*pos)--;
		fflush(stdout);
	}
}

void	handle_right_arrow(char *input, int *pos, char *prompt)
{
	int	len;

	(void)prompt;
	len = ft_strlen(input);
	if (*pos < len)
	{
		printf("%c", input[*pos]);
		(*pos)++;
		fflush(stdout);
	}
}

void	handle_backspace(char *input, int *pos, char *prompt)
{
	int	len;
	int	i;

	len = ft_strlen(input);
	if (len > 0 && *pos > 0)
	{
		/* Move all characters after cursor position one position left */
		i = *pos - 1;
		while (i < len - 1)
		{
			input[i] = input[i + 1];
			i++;
		}
		input[len - 1] = '\0';
		(*pos)--;
		/* Refresh the line to show the change */
		refresh_line(input, *pos, prompt);
	}
}

void	handle_char(char *input, int *pos, char c, char *prompt)
{
	int	len;
	int	i;

	len = ft_strlen(input);
	if (len < MAX_INPUT_SIZE - 1)
	{
		/* Move all characters from cursor position to the right */
		i = len;
		while (i > *pos)
		{
			input[i] = input[i - 1];
			i--;
		}
		/* Insert new character at cursor position */
		input[*pos] = c;
		(*pos)++;
		/* Refresh the line to show the change */
		refresh_line(input, *pos, prompt);
	}
}

/* display_prompt removed; use prompt string passed to readline */
