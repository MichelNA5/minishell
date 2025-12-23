/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_matches(char *dir_path, char *base_prefix)
{
	DIR				*dir;
	struct dirent	*ent;

	dir = opendir(dir_path);
	if (!dir)
		return ;
	while (1)
	{
		ent = readdir(dir);
		if (!ent)
			break ;
		if (ent->d_name[0] == '.')
			continue ;
		if (ft_strncmp(ent->d_name, base_prefix,
				ft_strlen(base_prefix)) == 0)
			ft_putendl_fd(ent->d_name, STDOUT_FILENO);
	}
	closedir(dir);
}

void	free_tab_parts(t_tab_ctx *ctx)
{
	free(ctx->parts[0]);
	free(ctx->parts[1]);
}

void	free_tab_ctx(t_tab_ctx *ctx)
{
	free(ctx->common);
	free(ctx->parts[0]);
	free(ctx->parts[1]);
	free(ctx->dir_path);
}

char	*get_dir_path(char *dir_part)
{
	char	*dir_path;

	if (dir_part[0] == '\0')
		dir_path = ft_strdup(".");
	else
	{
		if (dir_part[ft_strlen(dir_part) - 1] == '/'
			&& ft_strlen(dir_part) > 1)
			dir_path = ft_substr(dir_part, 0, ft_strlen(dir_part) - 1);
		else
			dir_path = ft_strdup(dir_part);
	}
	return (dir_path);
}
