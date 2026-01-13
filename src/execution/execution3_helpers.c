/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/08 15:39:45 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_pipeline(t_parser *parser, t_shell *shell)
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

void	exec_pipe_child(t_parser *parser, int i, t_shell *shell)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (i > 0)
	{
		dup2(parser->cmds[i].pipe_in, STDIN_FILENO);
		close(parser->cmds[i].pipe_in);
	}
	if (i < parser->cmd_count - 1)
	{
		dup2(parser->cmds[i].pipe_out, STDOUT_FILENO);
		close(parser->cmds[i].pipe_out);
	}
	close_pipes(parser);
	if (setup_redirections(&parser->cmds[i], shell) == -1)
	{
		cleanup_child_process(parser, shell);
		exit(1);
	}
	if (is_builtin(parser->cmds[i].args[0]))
		execute_builtin(&parser->cmds[i], shell);
	else
		execute_external(&parser->cmds[i], shell);
	cleanup_child_process(parser, shell);
	exit(shell->exit_status);
}

void	handle_last_child_exit(int status, t_shell *shell)
{
	int	sig;

	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGPIPE)
			shell->exit_status = 0;
		else
			shell->exit_status = 128 + sig;
	}
}
