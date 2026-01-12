/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/25 16:29:39 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_redir_out(char *file, int append)
{
	int	fd;

	if (append)
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_single_redir(t_cmd *cmd, int i, t_shell *shell)
{
	(void)shell;
	if (cmd->redirs[i].type == REDIR_IN)
	{
		if (handle_redir_in(cmd->redirs[i].file) == -1)
			return (-1);
	}
	else if (cmd->redirs[i].type == REDIR_OUT)
	{
		if (handle_redir_out(cmd->redirs[i].file, 0) == -1)
			return (-1);
	}
	else if (cmd->redirs[i].type == REDIR_APPEND)
	{
		if (handle_redir_out(cmd->redirs[i].file, 1) == -1)
			return (-1);
	}
	else if (cmd->redirs[i].type == REDIR_HEREDOC)
	{
		if (cmd->redirs[i].heredoc_fd != -1)
		{
			dup2(cmd->redirs[i].heredoc_fd, STDIN_FILENO);
			close(cmd->redirs[i].heredoc_fd);
		}
	}
	return (0);
}

int	setup_redirections(t_cmd *cmd, t_shell *shell)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (handle_single_redir(cmd, i, shell) == -1)
			return (-1);
		i++;
	}
	return (0);
}

void	restore_redirections(void)
{
	dup2(STDIN_FILENO, STDIN_FILENO);
	dup2(STDOUT_FILENO, STDOUT_FILENO);
}
