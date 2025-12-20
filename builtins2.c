/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/19 13:38:35 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_cmd *cmd)
{
	int	i;

	(void)cmd;
	i = 0;
	while (g_env && g_env[i])
	{
		ft_putendl_fd(g_env[i], STDOUT_FILENO);
		i++;
	}
	return;
}

void	builtin_exit(t_cmd *cmd)
{
	int	exit_code;

	exit_code = 0;
	if (cmd->args[1])
		exit_code = ft_atoi(cmd->args[1]);
	write(STDOUT_FILENO, "exit\n", 5);
	g_exit_status = exit_code;
	g_should_exit = 1;
}

void	print_env_vars(void)
{
	int			i;

	i = 0;
	while (g_env && g_env[i])
	{
		printf("declare -x %s\n", g_env[i]);
		i++;
	}
}
