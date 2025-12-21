/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:24:36 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		builtin_echo(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		builtin_cd(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
		builtin_pwd(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "export") == 0)
		builtin_export(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		builtin_unset(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		builtin_env(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		builtin_exit(cmd, shell);
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
