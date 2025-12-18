/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmakhlou <mmakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by mmakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_executable(char *cmd)
{
	char	*path;
	char	**path_dirs;
	char	*exec_path;
	int		i;

	if (access(cmd, X_OK) == 0)
		return (ft_strdup(cmd));
	path = get_env_var("PATH", g_env);
	if (!path)
		return (NULL);
	path_dirs = ft_split(path, ':');
	if (!path_dirs)
		return (NULL);
	i = 0;
	while (path_dirs[i])
	{
		exec_path = ft_strjoin(path_dirs[i], "/");
		exec_path = ft_strjoin(exec_path, cmd);
		if (access(exec_path, X_OK) == 0)
		{
			free_array(path_dirs);
			return (exec_path);
		}
		free(exec_path);
		i++;
	}
	free_array(path_dirs);
	return (NULL);
}
