/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3_helpers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/08 15:39:45 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	wait_all_children(t_parser *parser, t_shell *shell, pid_t *pids)
{
	int	i;
	int	status;
	int	sig;
	int	newline_printed;

	i = 0;
	newline_printed = 0;
	while (i < parser->cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == parser->cmd_count - 1)
			handle_last_child_exit(status, shell);
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			handle_signal_output(sig, &newline_printed);
		}
		i++;
	}
}

void	execute_pipeline(t_parser *parser, t_shell *shell)
{
	pid_t	pids[1024];
	int		i;

	if (!validate_pipeline(parser, shell))
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
