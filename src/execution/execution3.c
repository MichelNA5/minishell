/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by naous            ###   ########.fr       */
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

	i = 0;
	while (i < parser->cmd_count)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
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
}
