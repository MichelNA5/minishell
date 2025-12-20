/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_history2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by mmakhlou          #+#    #+#             */
/*   Updated: 2025/12/19 13:16:58 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_cursor_pos;
char		*get_history_command(int direction);

static int	is_delim(char c)
{
	return (c == ' ' || c == '\t');
}

static int	insert_str(char *input, int *pos, char *str, char *prompt)
{
	int	len;
	int	ins_len;

	if (!str || !*str)
		return (0);
	len = ft_strlen(input);
	ins_len = ft_strlen(str);
	if (len + ins_len >= MAX_INPUT_SIZE)
		return (0);
	ft_memmove(&input[*pos + ins_len], &input[*pos], (len - *pos) + 1);
	ft_memcpy(&input[*pos], str, ins_len);
	*pos += ins_len;
	refresh_line(input, *pos, prompt);
	return (1);
}

static void	shrink_common_prefix(char *common, char *candidate)
{
	int	i;

	i = 0;
	while (common[i] && candidate[i] && common[i] == candidate[i])
		i++;
	common[i] = '\0';
}

static char	*join_path(char *dir, char *name)
{
	if (!dir || !*dir)
		return (ft_strdup(name));
	return (ft_strjoin(dir, name));
}

static int	path_is_dir(char *dir_part, char *name)
{
	char			*full;
	struct stat	st;
	int			ret;

	ret = 0;
	full = join_path(dir_part, name);
	if (!full)
		return (0);
	if (stat(full, &st) == 0 && S_ISDIR(st.st_mode))
		ret = 1;
	free(full);
	return (ret);
}

static void	print_matches(char *dir_path, char *base_prefix)
{
	DIR			*dir;
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
		if (ft_strncmp(ent->d_name, base_prefix, ft_strlen(base_prefix)) == 0)
			ft_putendl_fd(ent->d_name, STDOUT_FILENO);
	}
	closedir(dir);
}

void	handle_tab(char *input, int *pos, char *prompt)
{
	int			start;
	int			i;
	char			*token;
	char			*dir_part;
	char			*base_prefix;
	char			*dir_path;
	DIR			*dir;
	struct dirent	*ent;
	int			matches;
	char			*common;
	int			base_len;

	if (*pos <= 0)
		return ;
	start = *pos;
	while (start > 0 && !is_delim(input[start - 1]))
		start--;
	if (start == *pos)
		return ;
	token = ft_substr(input, start, *pos - start);
	if (!token)
		return ;
	i = ft_strlen(token);
	while (i > 0 && token[i - 1] != '/')
		i--;
	dir_part = ft_substr(token, 0, i);
	base_prefix = ft_substr(token, i, ft_strlen(token) - i);
	free(token);
	if (!dir_part || !base_prefix)
	{
		free(dir_part);
		free(base_prefix);
		return ;
	}
	if (dir_part[0] == '\0')
		dir_path = ft_strdup(".");
	else
	{
		if (dir_part[ft_strlen(dir_part) - 1] == '/' && ft_strlen(dir_part) > 1)
			dir_path = ft_substr(dir_part, 0, ft_strlen(dir_part) - 1);
		else
			dir_path = ft_strdup(dir_part);
	}
	if (!dir_path)
	{
		free(dir_part);
		free(base_prefix);
		return ;
	}
	dir = opendir(dir_path);
	if (!dir)
	{
		free(dir_part);
		free(base_prefix);
		free(dir_path);
		return ;
	}
	matches = 0;
	common = NULL;
	base_len = ft_strlen(base_prefix);
	while (1)
	{
		ent = readdir(dir);
		if (!ent)
			break ;
		if (ent->d_name[0] == '.')
			continue ;
		if (ft_strncmp(ent->d_name, base_prefix, base_len) != 0)
			continue ;
		if (!common)
			common = ft_strdup(ent->d_name);
		else
			shrink_common_prefix(common, ent->d_name);
		matches++;
	}
	closedir(dir);
	if (matches == 0 || !common)
	{
		free(common);
		free(dir_part);
		free(base_prefix);
		free(dir_path);
		return ;
	}
	if ((int)ft_strlen(common) > base_len)
		insert_str(input, pos, common + base_len, prompt);
	else if (matches == 1 && path_is_dir(dir_part, common))
		insert_str(input, pos, "/", prompt);
	else if (matches > 1)
	{
		write(STDOUT_FILENO, "\n", 1);
		print_matches(dir_path, base_prefix);
		refresh_line(input, *pos, prompt);
	}
	free(common);
	free(dir_part);
	free(base_prefix);
	free(dir_path);
}

void	handle_up_arrow(char *input, int *pos, char *prompt)
{
	char	*line;
	int		len;

	line = get_history_command(1);
	if (line)
	{
		len = ft_strlen(input);
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		while (len > 0)
		{
			write(STDOUT_FILENO, " ", 1);
			len--;
		}
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
		*pos = ft_strlen(input);
		write(STDOUT_FILENO, input, ft_strlen(input));
	}
}

void	handle_down_arrow(char *input, int *pos, char *prompt)
{
	char	*line;
	int		len;

	line = get_history_command(-1);
	if (line)
	{
		len = ft_strlen(input);
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		while (len > 0)
		{
			write(STDOUT_FILENO, " ", 1);
			len--;
		}
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		ft_strlcpy(input, line, MAX_INPUT_SIZE);
		*pos = ft_strlen(input);
		write(STDOUT_FILENO, input, ft_strlen(input));
	}
	else
	{
		len = ft_strlen(input);
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		while (len > 0)
		{
			write(STDOUT_FILENO, " ", 1);
			len--;
		}
		write(STDOUT_FILENO, "\r", 1);
		write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		ft_memset(input, 0, MAX_INPUT_SIZE);
		*pos = 0;
	}
}

void	handle_left_arrow(char *input, int *pos, char *prompt)
{
	(void)input;
	(void)prompt;
	if (*pos > 0)
	{
		write(STDOUT_FILENO, "\b", 1);
		(*pos)--;
	}
}

void	handle_right_arrow(char *input, int *pos, char *prompt)
{
	int	len;

	(void)prompt;
	len = ft_strlen(input);
	if (*pos < len)
	{
		write(STDOUT_FILENO, &input[*pos], 1);
		(*pos)++;
	}
}

void	handle_backspace(char *input, int *pos, char *prompt)
{
	int	len;

	len = ft_strlen(input);
	if (len > 0 && *pos > 0)
	{
		ft_memmove(&input[*pos - 1], &input[*pos], (len - *pos) + 1);
		(*pos)--;
		refresh_line(input, *pos, prompt);
	}
}

void	handle_char(char *input, int *pos, char c, char *prompt)
{
	int	len;

	len = ft_strlen(input);
	if (len < MAX_INPUT_SIZE - 1)
	{
		ft_memmove(&input[*pos + 1], &input[*pos], (len - *pos) + 1);
		input[*pos] = c;
		(*pos)++;
		refresh_line(input, *pos, prompt);
	}
}

/* display_prompt removed; use prompt string passed to readline */
