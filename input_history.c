/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:48:25 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		read_byte(int *out);
char	*read_non_tty(void);
int		setup_terminal(struct termios *old, struct termios *new);
void	handle_special_char(char *input, int ch, char *prompt,
			t_shell *shell);
int		process_input_loop(char *input, char *prompt, t_shell *shell);

static char	*init_input_buffer(char *prompt, struct termios *old_term,
	int termios_enabled)
{
	char	*input;

	if (prompt)
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	input = malloc(MAX_INPUT_SIZE);
	if (!input)
	{
		if (termios_enabled)
			tcsetattr(STDIN_FILENO, TCSANOW, old_term);
		return (NULL);
	}
	ft_memset(input, 0, MAX_INPUT_SIZE);
	return (input);
}

char	*enhanced_readline(char *prompt, t_shell *shell)
{
	char			*input;
	struct termios	old_term;
	struct termios	new_term;
	int				termios_enabled;
	int				result;

	if (!isatty(STDIN_FILENO))
		return (read_non_tty());
	termios_enabled = setup_terminal(&old_term, &new_term);
	input = init_input_buffer(prompt, &old_term, termios_enabled);
	if (!input)
		return (NULL);
	shell->cursor_pos = 0;
	result = process_input_loop(input, prompt, shell);
	if (result == -1)
	{
		free(input);
		if (termios_enabled)
			tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
		return (NULL);
	}
	if (termios_enabled)
		tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	return (input);
}
