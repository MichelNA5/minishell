/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/22 02:12:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;

	if (!cmd->args[1])
		return ;
	i = 1;
	while (cmd->args[i])
	{
		unset_env_var(cmd->args[i], shell);
		i++;
	}
}

void	builtin_env(t_cmd *cmd, t_shell *shell)
{
	int	i;

	(void)cmd;
	i = 0;
	while (shell->env && shell->env[i])
	{
		ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		i++;
	}
}

void	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]);
	write(STDOUT_FILENO, "exit\n", 5);
	shell->exit_status = exit_code;
	shell->should_exit = 1;
}

void	print_env_vars(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->env && shell->env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(shell->env[i], STDOUT_FILENO);
		i++;
	}
}
