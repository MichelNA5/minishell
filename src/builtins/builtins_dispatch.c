/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 13:12:40 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	int	status;

	status = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		status = builtin_echo(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		status = builtin_cd(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		status = builtin_pwd(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		status = builtin_export(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		status = builtin_unset(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		status = builtin_env(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		status = builtin_exit(cmd, shell);
	shell->exit_status = status;
	return (status);
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
