/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/22 13:21:17 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	newline;

	(void)shell;
	i = 1;
	newline = 1;
	while (cmd->args[i] && ft_strcmp(cmd->args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (cmd->args[i])
	{
		ft_putstr_fd(cmd->args[i], STDOUT_FILENO);
		if (cmd->args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

static char	*get_cd_path(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args[1])
	{
		path = get_env_var("HOME", shell->env);
		if (!path)
		{
			write(STDERR_FILENO, "minishell: cd: HOME not set\n",
				ft_strlen("minishell: cd: HOME not set\n"));
			return (NULL);
		}
		return (path);
	}
	return (cmd->args[1]);
}

static void	update_pwd_vars(char *old_pwd, char *new_pwd, t_shell *shell)
{
	if (old_pwd)
		set_env_var("OLDPWD", old_pwd, shell);
	if (new_pwd)
		set_env_var("PWD", new_pwd, shell);
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
}

int	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	if (cmd->args[1] && cmd->args[2])
	{
		write(STDERR_FILENO, "minishell: cd: too many arguments\n",
			ft_strlen("minishell: cd: too many arguments\n"));
		return (1);
	}
	path = get_cd_path(cmd, shell);
	if (!path)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		free(old_pwd);
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	update_pwd_vars(old_pwd, new_pwd, shell);
	return (0);
}

int	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	status;

	if (!cmd->args[1])
	{
		print_env_vars(shell);
		return (0);
	}
	status = 0;
	i = 1;
	while (cmd->args[i])
	{
		if (process_export_arg(cmd->args[i], shell))
			status = 1;
		i++;
	}
	return (status);
}
