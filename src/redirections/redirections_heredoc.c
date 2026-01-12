/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2026/01/13 00:31:30 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_heredoc_interrupt(int *fd, t_shell *shell, char *line)
{
	free(line);
	close(fd[0]);
	close(fd[1]);
	shell->exit_status = 130;
	g_signal_received = 0;
	setup_signals();
	write(STDOUT_FILENO, "\033[A\033[2K\r", 9);
	return (-1);
}

static char	*expand_heredoc_line(char *line, t_shell *shell, int quoted)
{
	char	*expanded;

	if (!quoted)
	{
		expanded = expand_env_vars(line, shell);
		free(line);
		return (expanded);
	}
	return (line);
}

static int	read_heredoc_lines(int *fd, char *delimiter, t_shell *shell,
	int quoted)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT)
			return (handle_heredoc_interrupt(fd, shell, line));
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line = expand_heredoc_line(line, shell, quoted);
		if (!line)
			return (handle_heredoc_interrupt(fd, shell, NULL));
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, t_shell *shell, int quoted)
{
	int		fd[2];
	int		result;

	setup_signals_heredoc();
	pipe(fd);
	result = read_heredoc_lines(fd, delimiter, shell, quoted);
	if (result == -1)
		return (-1);
	close(fd[1]);
	setup_signals();
	return (fd[0]);
}

int	process_heredocs(t_parser *parser, t_shell *shell)
{
	int	i;
	int	j;
	int	fd;

	i = 0;
	while (i < parser->cmd_count)
	{
		j = 0;
		while (j < parser->cmds[i].redir_count)
		{
			if (parser->cmds[i].redirs[j].type == REDIR_HEREDOC)
			{
				fd = handle_heredoc(parser->cmds[i].redirs[j].file, shell,
						parser->cmds[i].redirs[j].quoted_delimiter);
				if (fd == -1)
					return (-1);
				parser->cmds[i].redirs[j].heredoc_fd = fd;
			}
			j++;
		}
		i++;
	}
	return (0);
}
