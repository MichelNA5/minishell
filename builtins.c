/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/19 13:16:58 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_echo(t_cmd *cmd, t_shell *shell)
{
	int	i;
	int	newline;

	(void)shell;
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
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
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

void	builtin_cd(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	*old_pwd;
	char	*new_pwd;

	path = get_cd_path(cmd, shell);
	if (!path)
		return ;
	old_pwd = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		free(old_pwd);
		return ;
	}
	new_pwd = getcwd(NULL, 0);
	if (old_pwd)
		set_env_var("OLDPWD", old_pwd, shell);
	if (new_pwd)
		set_env_var("PWD", new_pwd, shell);
	if (old_pwd)
		free(old_pwd);
	if (new_pwd)
		free(new_pwd);
}

void	builtin_pwd(t_cmd *cmd, t_shell *shell)
{
	char	*pwd;

	(void)cmd;
	(void)shell;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror("pwd");
		return ;
	}
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
}

void	builtin_export(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;
	char	*equal;

	if (!cmd->args[1])
	{
		print_env_vars(shell);
		return ;
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
			set_env_var(name, value, shell);
		}
		i++;
	}
}
