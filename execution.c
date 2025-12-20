/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/20 13:42:28 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_cmd *cmd, t_parser *parser)
{
	pid_t	pid;
	int		status;
	int		original_stdout;
	int		original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (!cmd || !cmd->args || !cmd->args[0])
	{
		g_exit_status = 2;
		return ;
	}
	if (setup_redirections(cmd) == -1)
	{
		restore_redirections();
		dup2(original_stdin, STDIN_FILENO);
		dup2(original_stdout, STDOUT_FILENO);
		close(original_stdin);
		close(original_stdout);
		g_exit_status = 1;
		return ;
	}
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd);
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			close(original_stdin);
			close(original_stdout);
			execute_external(cmd);
			cleanup_child_process(parser);
			exit(g_exit_status);
		}
		else if (pid < 0)
			perror("fork");
		else
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
		}
	}
	restore_redirections();
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	execute_commands(t_parser *parser)
{
	if (parser->cmd_count == 1)
		execute_command(&parser->cmds[0], parser);
	else
		execute_pipeline(parser);
}

void	execute_pipeline(t_parser *parser)
{
	pid_t	pid;
	int		status;
	int		i;

	i = 0;
	while (i < parser->cmd_count)
	{
		if (!parser->cmds[i].args || !parser->cmds[i].args[0])
		{
			g_exit_status = 2;
			return ;
		}
		i++;
	}
	setup_pipes(parser);
	i = 0;
	while (i < parser->cmd_count)
	{
		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
				dup2(parser->cmds[i].pipe_in, STDIN_FILENO);
			if (i < parser->cmd_count - 1)
				dup2(parser->cmds[i].pipe_out, STDOUT_FILENO);
			close_pipes(parser);
			if (is_builtin(parser->cmds[i].args[0]))
				execute_builtin(&parser->cmds[i]);
			else
				execute_external(&parser->cmds[i]);
			cleanup_child_process(parser);
			exit(g_exit_status);
		}
		else if (pid < 0)
			perror("fork");
		i++;
	}
	close_pipes(parser);
	i = 0;
	while (i < parser->cmd_count)
	{
		waitpid(-1, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

void	execute_external(t_cmd *cmd)
{
	char	*exec_path;
	char	**env;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		g_exit_status = 127;
		return ;
	}
	exec_path = find_executable(cmd->args[0]);
	if (!exec_path)
	{
		printf("minishell: %s: command not found\n", cmd->args[0]);
		g_exit_status = 127;
		return ;
	}
	env = g_env;
	if (execve(exec_path, cmd->args, env) == -1)
	{
		perror("execve");
		g_exit_status = 126;
	}
	free(exec_path);
}

void	execute_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(cmd);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(cmd);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(cmd);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	setup_redirections(t_cmd *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_IN)
		{
			fd = open(cmd->redirs[i].file, O_RDONLY);
			if (fd == -1)
			{
				perror("open");
				return (-1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_OUT)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror("open");
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_APPEND)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror("open");
				return (-1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
			handle_heredoc(cmd->redirs[i].file);
		i++;
	}
	return (0);
}

void	handle_heredoc(char *delimiter)
{
	char	*line;
	int		fd[2];

	pipe(fd);
	while (1)
	{
		line = enhanced_readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
}

void	restore_redirections(void)
{
	dup2(STDIN_FILENO, STDIN_FILENO);
	dup2(STDOUT_FILENO, STDOUT_FILENO);
}
