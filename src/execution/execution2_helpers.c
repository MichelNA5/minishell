/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/08 15:53:12 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_path_dir(char *dir, char *cmd)
{
	char			*tmp;
	char			*exec_path;
	struct stat		st;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	exec_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!exec_path)
		return (NULL);
	if (stat(exec_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		free(exec_path);
		return (NULL);
	}
	if (access(exec_path, X_OK) == 0)
		return (exec_path);
	free(exec_path);
	return (NULL);
}

static char	*search_in_path(char **path_dirs, char *cmd)
{
	char	*result;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		result = check_path_dir(path_dirs[i], cmd);
		if (result)
		{
			free_array(path_dirs);
			return (result);
		}
		i++;
	}
	free_array(path_dirs);
	return (NULL);
}

char	*find_executable(char *cmd, t_shell *shell)
{
	char	*path;
	char	**path_dirs;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path = get_env_var("PATH", shell->env);
	if (!path)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
		return (NULL);
	return (search_in_path(path_dirs, cmd));
}

void	restore_fds(int *fds)
{
	restore_redirections();
	dup2(fds[0], STDIN_FILENO);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[0]);
	close(fds[1]);
}

int	prepare_command_execution(t_cmd *cmd, t_parser *parser,
	t_shell *shell, int *fds)
{
	(void)cmd;
	if (process_heredocs(parser, shell) == -1)
	{
		restore_fds(fds);
		return (-1);
	}
	if (setup_redirections(cmd, shell) == -1)
	{
		restore_fds(fds);
		if (shell->exit_status != 130)
			shell->exit_status = 1;
		return (-1);
	}
	return (0);
}
