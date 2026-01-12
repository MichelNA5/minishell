/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/12 00:00:00 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler_heredoc(int sig)
{
	char	newline;

	newline = '\n';
	g_signal_received = sig;
	ioctl(STDIN_FILENO, TIOCSTI, &newline);
}

void	setup_signals_heredoc(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
