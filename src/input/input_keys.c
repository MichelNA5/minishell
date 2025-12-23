/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_keys.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:52:26 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	read_byte(int *out);

static void	handle_sigint(char *input, char *prompt, t_shell *shell)
{
	write(STDOUT_FILENO, "\n", 1);
	ft_memset(input, 0, MAX_INPUT_SIZE);
	shell->cursor_pos = 0;
	if (prompt)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
}

static void	handle_escape_seq(char *input, char *prompt, t_shell *shell)
{
	int	key;

	if (read_byte(&key) <= 0)
		return ;
	if (key != 91)
		return ;
	if (read_byte(&key) <= 0)
		return ;
	if (key == 65)
		handle_up_arrow(input, &shell->cursor_pos, prompt, shell);
	else if (key == 66)
		handle_down_arrow(input, &shell->cursor_pos, prompt, shell);
	else if (key == 67)
		handle_right_arrow(input, &shell->cursor_pos, prompt);
	else if (key == 68)
		handle_left_arrow(input, &shell->cursor_pos, prompt);
}

void	handle_special_char(char *input, int ch, char *prompt, t_shell *shell)
{
	if (ch == 4 && ft_strlen(input) == 0)
		return ;
	if (ch == 3)
	{
		handle_sigint(input, prompt, shell);
		return ;
	}
	if (ch == 27)
	{
		handle_escape_seq(input, prompt, shell);
		return ;
	}
	if (ch == 127 || ch == 8)
		handle_backspace(input, &shell->cursor_pos, prompt);
	else if (ch == '\t')
		handle_tab(input, &shell->cursor_pos, prompt);
	else if (ch >= 32 && ch <= 126)
		handle_char(input, &shell->cursor_pos, (char)ch, prompt);
}
