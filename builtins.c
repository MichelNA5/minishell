/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/11 23:42:56 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (cmd->args[1] && ft_strcmp(cmd->args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (cmd->args[i])
	{
		printf("%s", cmd->args[i]);
		if (cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	(void)cmd;
	return;
}

void	builtin_cd(t_cmd *cmd)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (!cmd->args[1])
	{
		path = get_env_var("HOME", NULL);
		if (!path)
		{
			printf("minishell: cd: HOME not set\n");
			return;
		}
	}
	else
		path = cmd->args[1];
	old_pwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return;
	}
	new_pwd = getcwd(NULL, 0);
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
	return;
}

void	builtin_pwd(t_cmd *cmd)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return;
	}
	printf("%s\n", pwd);
	free(pwd);
	(void)cmd;
	return;
}

void	builtin_export(t_cmd *cmd)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal;

	if (!cmd->args[1])
	{
		print_env_vars();
		return;
	}
	i = 1;
	while (cmd->args[i])
	{
		equal = ft_strchr(cmd->args[i], '=');
		if (equal)
		{
			*equal = '\0';
			name = cmd->args[i];
			value = equal + 1;
			set_env_var(name, value);
		}
		i++;
	}
	return;
}

void	builtin_unset(t_cmd *cmd)
{
	int	i;

	if (!cmd->args[1])
		return;
	i = 1;
	while (cmd->args[i])
	{
		unset_env_var(cmd->args[i]);
		i++;
	}
	return;
}
