/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 13:19:54 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || name[0] == '\0')
		return (0);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	print_export_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
}

static int	process_export_noequal(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		print_export_error(arg);
		return (1);
	}
	return (0);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	char	*equal;
	char	*name;
	int		status;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (process_export_noequal(arg));
	name = ft_substr(arg, 0, (size_t)(equal - arg));
	if (!name)
		return (1);
	status = 0;
	if (!is_valid_identifier(name))
	{
		print_export_error(arg);
		status = 1;
	}
	else
		set_env_var(name, equal + 1, shell);
	free(name);
	return (status);
}
