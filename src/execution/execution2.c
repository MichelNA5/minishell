/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/08 15:53:12 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_path_dir(char *dir, char *cmd)
{
	char	*tmp;
	char	*exec_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	exec_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!exec_path)
		return (NULL);
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

void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*exec_path;
	char	*sh_args[3];

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = 127;
		return ;
	}
	exec_path = find_executable(cmd->args[0], shell);
	if (!exec_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		shell->exit_status = 127;
		return ;
	}
	if (access(exec_path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		free(exec_path);
		shell->exit_status = 126;
		return ;
	}
	if (execve(exec_path, cmd->args, shell->env) == -1)
	{
		if (errno == ENOEXEC)
		{
			sh_args[0] = "/bin/sh";
			sh_args[1] = exec_path;
			sh_args[2] = NULL;
			execve("/bin/sh", sh_args, shell->env);
		}
		perror("execve");
		shell->exit_status = 126;
	}
	free(exec_path);
}
