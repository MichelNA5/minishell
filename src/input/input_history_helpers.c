/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:52:36 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	read_byte(int *out)
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

int	setup_terminal(struct termios *old, struct termios *new)
{
	if (!isatty(STDIN_FILENO))
		return (0);
	if (tcgetattr(STDIN_FILENO, old) != 0)
		return (0);
	*new = *old;
	new->c_lflag &= ~(ICANON | ECHO);
	new->c_cc[VMIN] = 1;
	new->c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, new) != 0)
		return (0);
	return (1);
}
