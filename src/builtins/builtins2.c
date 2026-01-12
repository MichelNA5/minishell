/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/22 13:21:38 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_cmd *cmd, t_shell *shell)
{
	int	i;

	(void)shell;
	if (!cmd->args[1])
		return (0);
	i = 1;
	while (cmd->args[i])
	{
		unset_env_var(cmd->args[i], shell);
		i++;
	}
	return (0);
}

static int	handle_exit_no_args(t_shell *shell)
{
	shell->exit_status = 0;
	shell->should_exit = 1;
	return (shell->exit_status);
}

static int	handle_exit_too_many(t_shell *shell)
{
	ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
	shell->exit_status = 1;
	shell->should_exit = 0;
	return (shell->exit_status);
}

static int	handle_exit_nonnumeric(char *arg, t_shell *shell)
{
	print_exit_numeric_error(arg);
	shell->exit_status = 2;
	shell->should_exit = 1;
	return (shell->exit_status);
}

int	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	long long	exit_ll;
	int			exit_code;

	write(STDOUT_FILENO, "exit\n", 5);
	if (!cmd->args[1])
		return (handle_exit_no_args(shell));
	if (cmd->args[2])
		return (handle_exit_too_many(shell));
	if (!parse_ll(cmd->args[1], &exit_ll))
		return (handle_exit_nonnumeric(cmd->args[1], shell));
	exit_code = (unsigned char)exit_ll;
	shell->exit_status = exit_code;
	shell->should_exit = 1;
	return (shell->exit_status);
}
