/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/18 15:02:07 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
extern int	g_cursor_pos;

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
		printf("%s\n", message);
}

void	refresh_line(char *line, int pos, char *prompt)
{
	int	len;
	int	i;

	len = ft_strlen(line);
	printf("\r\033[2K%s%s", prompt, line);
	i = 0;
	while (i < len - pos)
	{
		printf("\b");
		i++;
	}
	fflush(stdout);
}

char	*enhanced_readline(char *prompt)
{
	char            *input;
	int             ch;
	struct termios  old_term;
	struct termios  new_term;

	tcgetattr(STDIN_FILENO, &old_term);
	new_term = old_term;
	new_term.c_lflag &= ~(ICANON | ECHO);
	new_term.c_cc[VMIN] = 1;
	new_term.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	printf("%s", prompt);
	fflush(stdout);
	input = malloc(MAX_INPUT_SIZE);
	if (!input)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
		return (NULL);
	}
	ft_memset(input, 0, MAX_INPUT_SIZE);
	g_cursor_pos = 0;
	while (1)
	{
		ch = getchar();
		if (ch == '\n')
		{
			printf("\n");
			break;
		}
		else if (ch == 4)
		{
			if (ft_strlen(input) == 0)
			{
				free(input);
				tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
				return (NULL);
			}
		}
		else if (ch == 3)
		{
			printf("\n");
			ft_memset(input, 0, MAX_INPUT_SIZE);
			g_cursor_pos = 0;
			printf("%s", prompt);
			fflush(stdout);
			continue;
		}
		else if (ch == 27)
		{
			ch = getchar();
			if (ch == 91)
			{
				ch = getchar();
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
	tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	return (input);
}
