/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/08 15:39:45 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <errno.h>

void	handle_signal_output(int sig, int *newline_printed)
{
	if (sig == SIGINT && !*newline_printed)
	{
		write(STDOUT_FILENO, "\n", 1);
		*newline_printed = 1;
	}
	else if (sig == SIGQUIT && !*newline_printed)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		*newline_printed = 1;
	}
}

static int	check_interrupted_child(int status, pid_t pid, pid_t last_pid,
	int *newline_printed)
{
	int	sig;

	if (!WIFSIGNALED(status))
		return (0);
	sig = WTERMSIG(status);
	if (sig == SIGINT || sig == SIGQUIT)
	{
		if (pid == last_pid)
			handle_signal_output(sig, newline_printed);
		return (sig);
	}
	return (0);
}

static void	set_exit_status_from_wait(int interrupt_sig, int last_status,
	t_shell *shell)
{
	if (interrupt_sig)
		shell->exit_status = 128 + interrupt_sig;
	else
		handle_last_child_exit(last_status, shell);
}

static int	wait_single_child(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		if (errno == EINTR)
			return (-1);
		return (0);
	}
	return (1);
}

void	wait_all_children(t_parser *parser, t_shell *shell, pid_t *pids)
{
	int		i;
	int		status;
	int		last_status;
	int		newline_printed;
	int		interrupt_sig;

	i = -1;
	newline_printed = 0;
	last_status = 0;
	interrupt_sig = 0;
	while (++i < parser->cmd_count)
	{
		if (wait_single_child(pids[i], &status) == -1)
		{
			i--;
			continue ;
		}
		if (!interrupt_sig)
			interrupt_sig = check_interrupted_child(status, pids[i],
					pids[parser->cmd_count - 1], &newline_printed);
		if (pids[i] == pids[parser->cmd_count - 1])
			last_status = status;
	}
	set_exit_status_from_wait(interrupt_sig, last_status, shell);
}
