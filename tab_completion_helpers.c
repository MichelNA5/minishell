/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tab_completion_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/20 00:00:00 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delim(char c)
{
	return (c == ' ' || c == '\t');
}

int	insert_str(char *input, int *pos, char *str, char *prompt)
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

void	shrink_common_prefix(char *common, char *candidate)
{
	int	i;

	i = 0;
	while (common[i] && candidate[i] && common[i] == candidate[i])
		i++;
	common[i] = '\0';
}

char	*join_path(char *dir, char *name)
{
	if (!dir || !*dir)
		return (ft_strdup(name));
	return (ft_strjoin(dir, name));
}

int	path_is_dir(char *dir_part, char *name)
{
	char		*full;
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
