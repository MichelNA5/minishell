/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/07 12:52:51 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_pipeline(t_parser *parser, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < parser->cmd_count)
	{
		if (!parser->cmds[i].args || !parser->cmds[i].args[0])
		{
			shell->exit_status = 2;
			return (0);
		}
		i++;
	}
	return (1);
}

static void	exec_pipe_child(t_parser *parser, int i, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (i > 0)
		dup2(parser->cmds[i].pipe_in, STDIN_FILENO);
	if (i < parser->cmd_count - 1)
		dup2(parser->cmds[i].pipe_out, STDOUT_FILENO);
	close_pipes(parser);
	if (is_builtin(parser->cmds[i].args[0]))
		execute_builtin(&parser->cmds[i], shell);
	else
		execute_external(&parser->cmds[i], shell);
	cleanup_child_process(parser, shell);
	exit(shell->exit_status);
}

static void	wait_all_children(t_parser *parser, t_shell *shell)
{
	int	i;
	int	status;
	int	sig;
	int	newline_printed;

	i = 0;
	newline_printed = 0;
	while (i < parser->cmd_count)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			shell->exit_status = 128 + sig;
			if (sig == SIGINT && !newline_printed)
			{
				write(STDOUT_FILENO, "\n", 1);
				newline_printed = 1;
			}
			else if (sig == SIGQUIT && !newline_printed)
			{
				write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
				newline_printed = 1;
			}
		}
		i++;
	}
}

void	execute_pipeline(t_parser *parser, t_shell *shell)
{
	pid_t	pid;
	int		i;

	if (!validate_pipeline(parser, shell))
		return ;
	setup_pipes(parser);
	set_signals_for_execution();
	i = 0;
	while (i < parser->cmd_count)
	{
		pid = fork();
		if (pid == 0)
			exec_pipe_child(parser, i, shell);
		else if (pid < 0)
			perror("fork");
		i++;
	}
	close_pipes(parser);
	wait_all_children(parser, shell);
	restore_signals_after_execution();
}
