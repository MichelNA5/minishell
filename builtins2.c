/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:43:01 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_env(t_cmd *cmd)
{
	extern char **environ;
	int	i;

	(void)cmd;
	i = 0;
	while (environ && environ[i])
	{
		printf("%s\n", environ[i]);
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
	printf("exit\n");
	exit(exit_code);
}

void	print_env_vars(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		printf("declare -x %s\n", environ[i]);
		i++;
	}
}
