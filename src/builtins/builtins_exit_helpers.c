/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/22 13:19:54 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

int	is_numeric_str(const char *s)
{
	int	i;

	if (!s || s[0] == '\0')
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	check_overflow(long long result, int sign, char digit)
{
	if (sign == 1 && (result > (LLONG_MAX - (digit - '0')) / 10))
		return (0);
	if (sign == -1 && (-result < (LLONG_MIN + (digit - '0')) / 10))
		return (0);
	return (1);
}

int	parse_ll(const char *s, long long *out)
{
	long long	result;
	int			sign;
	int			i;

	if (!is_numeric_str(s))
		return (0);
	sign = 1;
	i = 0;
	if (s[i] == '+' || s[i] == '-')
	{
		if (s[i] == '-')
			sign = -1;
		i++;
	}
	result = 0;
	while (s[i])
	{
		if (!check_overflow(result, sign, s[i]))
			return (0);
		result = result * 10 + (s[i] - '0');
		i++;
	}
	*out = result * sign;
	return (1);
}

void	print_exit_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}
