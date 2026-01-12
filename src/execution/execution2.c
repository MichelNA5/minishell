/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 12:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2026/01/08 15:53:12 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_command_error(char *cmd, char *error_msg, t_shell *shell,
	int exit_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putendl_fd(error_msg, STDERR_FILENO);
	shell->exit_status = exit_code;
}

static void	handle_exec_path_not_found(char *cmd, int has_slash, t_shell *shell)
{
	if (has_slash)
		print_command_error(cmd, ": No such file or directory", shell, 127);
	else
		print_command_error(cmd, ": command not found", shell, 127);
}

static int	validate_exec_path(char *exec_path, char *cmd, t_shell *shell)
{
	struct stat	st;

	if (stat(exec_path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		print_command_error(cmd, ": Is a directory", shell, 126);
		free(exec_path);
		return (0);
	}
	if (access(exec_path, X_OK) != 0)
	{
		print_command_error(cmd, ": Permission denied", shell, 126);
		free(exec_path);
		return (0);
	}
	return (1);
}

static void	handle_execve_error(char *exec_path, t_shell *shell)
{
	char	*sh_args[3];

	if (errno == ENOEXEC)
	{
		sh_args[0] = "/bin/sh";
		sh_args[1] = exec_path;
		sh_args[2] = NULL;
		execve("/bin/sh", sh_args, shell->env);
	}
	perror("execve");
	shell->exit_status = 126;
}

void	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*exec_path;
	int		has_slash;

	if (!cmd || !cmd->args || !cmd->args[0])
	{
		shell->exit_status = 127;
		return ;
	}
	has_slash = (ft_strchr(cmd->args[0], '/') != NULL);
	exec_path = find_executable(cmd->args[0], shell);
	if (!exec_path)
	{
		handle_exec_path_not_found(cmd->args[0], has_slash, shell);
		return ;
	}
	if (!validate_exec_path(exec_path, cmd->args[0], shell))
		return ;
	if (execve(exec_path, cmd->args, shell->env) == -1)
		handle_execve_error(exec_path, shell);
	free(exec_path);
}
