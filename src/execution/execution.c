/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/07 12:45:53 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_child_process(t_cmd *cmd, t_parser *parser, t_shell *shell,
	int *fds)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	close(fds[0]);
	close(fds[1]);
	execute_external(cmd, shell);
	cleanup_child_process(parser, shell);
	exit(shell->exit_status);
}

static void	handle_parent_process(int *status, t_shell *shell)
{
	int	sig;

	if (WIFEXITED(*status))
		shell->exit_status = WEXITSTATUS(*status);
	else if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		shell->exit_status = 128 + sig;
		if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
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
		set_signals_for_execution();
		waitpid(pid, &status, 0);
		restore_signals_after_execution();
		handle_parent_process(&status, shell);
	}
}

void	execute_command(t_cmd *cmd, t_parser *parser, t_shell *shell)
{
	int	fds[2];

	fds[0] = dup(STDIN_FILENO);
	fds[1] = dup(STDOUT_FILENO);
	if (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		if (fds[0] != -1)
			close(fds[0]);
		if (fds[1] != -1)
			close(fds[1]);
		shell->exit_status = 0;
		return ;
	}
	if (prepare_command_execution(cmd, parser, shell, fds) == -1)
		return ;
	if (is_builtin(cmd->args[0]))
		execute_builtin(cmd, shell);
	else
		execute_external_fork(cmd, parser, shell, fds);
	restore_fds(fds);
}
