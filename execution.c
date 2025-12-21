/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/21 18:24:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_cmd *cmd, t_parser *parser, t_shell *shell,
	int *fds)
{
	close(fds[0]);
	close(fds[1]);
	execute_external(cmd, shell);
	cleanup_child_process(parser, shell);
	exit(shell->exit_status);
}

static void	handle_parent_process(int *status, t_shell *shell)
{
	if (WIFEXITED(*status))
		shell->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
		shell->exit_status = 128 + WTERMSIG(*status);
}

static void	restore_fds(int *fds)
{
	restore_redirections();
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}

static void	execute_external_fork(t_cmd *cmd, t_parser *parser,
	t_shell *shell, int *fds)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		handle_child_process(cmd, parser, shell, fds);
	else if (pid < 0)
		perror("fork");
	else
	{
		waitpid(pid, &status, 0);
		handle_parent_process(&status, shell);
	}
}

void	execute_command(t_cmd *cmd, t_parser *parser, t_shell *shell)
{
	int	fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = 2;
		return ;
	}
	if (setup_redirections(cmd, shell) == -1)
	{
		restore_fds(fds);
		shell->exit_status = 1;
		return ;
	}
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, shell);
	else
		execute_external_fork(cmd, parser, shell, fds);
	restore_fds(fds);
}
