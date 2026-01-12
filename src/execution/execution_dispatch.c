/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_dispatch.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/21 12:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/21 18:24:36 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_commands(t_parser *parser, t_shell *shell)
{
	if (parser->cmd_count == 1)
		execute_command(&parser->cmds[0], parser, shell);
	else
		execute_pipeline(parser, shell);
}
