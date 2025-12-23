/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	refresh_line(char *line, int pos, char *prompt);
int		is_delim(char c);
int		insert_str(char *input, int *pos, char *str, char *prompt);
void	shrink_common_prefix(char *common, char *candidate);
char	*join_path(char *dir, char *name);
int		path_is_dir(char *dir_part, char *name);
void	print_matches(char *dir_path, char *base_prefix);
void	free_tab_parts(t_tab_ctx *ctx);
void	free_tab_ctx(t_tab_ctx *ctx);
char	*get_dir_path(char *dir_part);

static void	complete_tab_part1(char *input, int *pos, char **parts)
{
	int		start;
	int		i;
	char	*token;

	start = *pos;
	while (start > 0 && !is_delim(input[start - 1]))
		start--;
	token = ft_substr(input, start, *pos - start);
	if (!token)
	{
		parts[0] = NULL;
		return ;
	}
	i = ft_strlen(token);
	while (i > 0 && token[i - 1] != '/')
		i--;
	parts[0] = ft_substr(token, 0, i);
	parts[1] = ft_substr(token, i, ft_strlen(token) - i);
	free(token);
}

void	handle_matches_helper(char *input, int *pos, char *prompt,
		t_tab_ctx *ctx)
{
	int	base_len;

	base_len = ft_strlen(ctx->parts[1]);
	if ((int)ft_strlen(ctx->common) > base_len)
		insert_str(input, pos, ctx->common + base_len, prompt);
	else if (ctx->matches == 1 && path_is_dir(ctx->parts[0], ctx->common))
		insert_str(input, pos, "/", prompt);
	else if (ctx->matches > 1)
	{
		write(STDOUT_FILENO, "\n", 1);
		print_matches(ctx->dir_path, ctx->parts[1]);
		refresh_line(input, *pos, prompt);
	}
}

static int	find_completions(DIR *dir, t_tab_ctx *ctx)
{
	struct dirent	*ent;

	ctx->matches = 0;
	ctx->common = NULL;
	while (1)
	{
		ent = readdir(dir);
		if (!ent)
			break ;
		if (ent->d_name[0] == '.')
			continue ;
		if (ft_strncmp(ent->d_name, ctx->parts[1],
				ft_strlen(ctx->parts[1])) != 0)
			continue ;
		if (!ctx->common)
			ctx->common = ft_strdup(ent->d_name);
		else
			shrink_common_prefix(ctx->common, ent->d_name);
		ctx->matches++;
	}
	return (ctx->matches);
}

static int	init_tab_ctx(t_tab_ctx *ctx, char *input, int *pos)
{
	if (*pos <= 0)
		return (0);
	complete_tab_part1(input, pos, ctx->parts);
	if (!ctx->parts[0] || !ctx->parts[1])
	{
		free_tab_parts(ctx);
		return (0);
	}
	ctx->dir_path = get_dir_path(ctx->parts[0]);
	if (!ctx->dir_path)
	{
		free_tab_parts(ctx);
		return (0);
	}
	return (1);
}

void	handle_tab(char *input, int *pos, char *prompt)
{
	t_tab_ctx	ctx;
	DIR			*dir;

	if (!init_tab_ctx(&ctx, input, pos))
		return ;
	dir = opendir(ctx.dir_path);
	if (!dir)
	{
		free_tab_parts(&ctx);
		free(ctx.dir_path);
		return ;
	}
	if (find_completions(dir, &ctx) == 0 || !ctx.common)
	{
		closedir(dir);
		free_tab_ctx(&ctx);
		return ;
	}
	closedir(dir);
	handle_matches_helper(input, pos, prompt, &ctx);
	free_tab_ctx(&ctx);
}
