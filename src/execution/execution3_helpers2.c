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

void	handle_last_child_exit(int status, t_shell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

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

static void	update_last_signal(int status, int *last_signal,
				int *newline_printed)
{
	int	sig;

	if (!WIFSIGNALED(status))
		return ;
	sig = WTERMSIG(status);
	*last_signal = sig;
	handle_signal_output(sig, newline_printed);
}

void	wait_all_children(t_parser *parser, t_shell *shell, pid_t *pids)
{
	int	i;
	int	status;
	int	newline_printed;
	int	last_signal;

	i = 0;
	newline_printed = 0;
	last_signal = 0;
	while (i < parser->cmd_count)
	{
		if (waitpid(pids[i], &status, 0) == -1)
		{
			if (errno == EINTR)
				continue ;
			break ;
		}
		update_last_signal(status, &last_signal, &newline_printed);
		i++;
	}
	if (last_signal != 0)
		shell->exit_status = 128 + last_signal;
	else
		handle_last_child_exit(status, shell);
}

void	execute_pipeline(t_parser *parser, t_shell *shell)
{
	pid_t	pids[1024];
	int		i;

	if (!validate_pipeline(parser, shell))
		return ;
	if (process_heredocs(parser, shell) == -1)
		return ;
	setup_pipes(parser);
	set_signals_for_execution();
	i = 0;
	while (i < parser->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			exec_pipe_child(parser, i, shell);
		else if (pids[i] < 0)
			perror("fork");
		i++;
	}
	close_pipes(parser);
	wait_all_children(parser, shell, pids);
	restore_signals_after_execution();
}
