/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/19 13:16:29 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern int	g_cursor_pos;

static int	read_byte(int *out)
{
	unsigned char	c;
	ssize_t			r;

	r = read(STDIN_FILENO, &c, 1);
	if (r == 1)
	{
		*out = (int)c;
		return (1);
	}
	if (r == 0)
		return (0);
	return (-1);
}

static char	*history[MAX_HISTORY_SIZE];
static int	history_count = 0;
static int	history_index = 0;

void	init_history(void)
{
	int i;

	i = 0;
	while (i < MAX_HISTORY_SIZE)
	{
		history[i] = NULL;
		i++;
	}
	history_count = 0;
	history_index = 0;
}

void	add_history_entry(char *line)
{
	if (!line)
		return;
	if (history_count < MAX_HISTORY_SIZE)
	{
		history[history_count] = ft_strdup(line);
		history_count++;
	}
	else
	{
		free(history[0]);
		int i = 0;
		while (i < MAX_HISTORY_SIZE - 1)
		{
			history[i] = history[i + 1];
			i++;
		}
		history[MAX_HISTORY_SIZE - 1] = ft_strdup(line);
	}
	history_index = history_count;
}

char	*get_history_command(int direction)
{
	if (direction > 0)
	{
		if (history_index > 0)
			history_index--;
	}
	else
	{
		if (history_index < history_count)
			history_index++;
	}
	if (history_index >= 0 && history_index < history_count)
		return (history[history_index]);
	return (NULL);
}

void	free_history(void)
{
	int i = 0;
	while (i < history_count)
	{
		free(history[i]);
		history[i] = NULL;
		i++;
	}
	history_count = 0;
	history_index = 0;
}

void	display_welcome(char *message)
{
	if (message)
		ft_putendl_fd(message, STDOUT_FILENO);
}

void	refresh_line(char *line, int pos, char *prompt)
{
	int	len;
	int	i;

	len = ft_strlen(line);
	write(STDOUT_FILENO, "\r\033[2K", 5);
	if (prompt)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	if (line)
		write(STDOUT_FILENO, line, ft_strlen(line));
	i = 0;
	while (i < len - pos)
	{
		write(STDOUT_FILENO, "\b", 1);
		i++;
	}
}

char	*enhanced_readline(char *prompt)
{
	char            *input;
	int             ch;
	struct termios  old_term;
	struct termios  new_term;
	int				termios_enabled;
	int				idx;
	int				status;

	if (!isatty(STDIN_FILENO))
	{
		input = malloc(MAX_INPUT_SIZE);
		if (!input)
			return (NULL);
		ft_memset(input, 0, MAX_INPUT_SIZE);
		idx = 0;
		while (idx < MAX_INPUT_SIZE - 1)
		{
			status = read_byte(&ch);
			if (status <= 0)
			{
				if (idx == 0)
				{
					free(input);
					return (NULL);
				}
				break;
			}
			if (ch == '\n')
				break;
			input[idx++] = (char)ch;
		}
		input[idx] = '\0';
		return (input);
	}

	termios_enabled = 0;
	if (isatty(STDIN_FILENO) && tcgetattr(STDIN_FILENO, &old_term) == 0)
	{
		new_term = old_term;
		new_term.c_lflag &= ~(ICANON | ECHO);
		new_term.c_cc[VMIN] = 1;
		new_term.c_cc[VTIME] = 0;
		if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) == 0)
			termios_enabled = 1;
	}
	if (prompt)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	input = malloc(MAX_INPUT_SIZE);
	if (!input)
	{
		if (termios_enabled)
			tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
		return (NULL);
	}
	ft_memset(input, 0, MAX_INPUT_SIZE);
	g_cursor_pos = 0;
	while (1)
	{
		status = read_byte(&ch);
		if (status <= 0)
		{
			if (ft_strlen(input) == 0)
			{
				free(input);
				if (termios_enabled)
					tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
				return (NULL);
			}
			break;
		}
		if (ch == '\n')
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		else if (ch == 4)
		{
			if (ft_strlen(input) == 0)
			{
				free(input);
				if (termios_enabled)
					tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
				return (NULL);
			}
		}
		else if (ch == 3)
		{
			write(STDOUT_FILENO, "\n", 1);
			ft_memset(input, 0, MAX_INPUT_SIZE);
			g_cursor_pos = 0;
			if (prompt)
				write(STDOUT_FILENO, prompt, ft_strlen(prompt));
			continue;
		}
		else if (ch == 27)
		{
			if (read_byte(&ch) <= 0)
				break;
			if (ch == 91)
			{
				if (read_byte(&ch) <= 0)
					break;
				if (ch == 65)
					handle_up_arrow(input, &g_cursor_pos, prompt);
				else if (ch == 66)
					handle_down_arrow(input, &g_cursor_pos, prompt);
				else if (ch == 67)
					handle_right_arrow(input, &g_cursor_pos, prompt);
				else if (ch == 68)
					handle_left_arrow(input, &g_cursor_pos, prompt);
			}
		}
		else if (ch == 127 || ch == 8)
		{
			handle_backspace(input, &g_cursor_pos, prompt);
		}
		else if (ch == '\t')
		{
			handle_tab(input, &g_cursor_pos, prompt);
		}
		else if (ch >= 32 && ch <= 126)
		{
			handle_char(input, &g_cursor_pos, (char)ch, prompt);
		}
	}
	if (termios_enabled)
		tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	return (input);
}
