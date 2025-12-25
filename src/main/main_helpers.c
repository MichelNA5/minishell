/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: naous <naous@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 00:00:00 by naous             #+#    #+#             */
/*   Updated: 2025/12/25 16:29:39 by naous            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*build_prompt(void)
{
	char	*cwd;
	char	*tmp;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell$ "));
	tmp = ft_strjoin("minishell:", cwd);
	free(cwd);
	if (!tmp)
		return (ft_strdup("minishell$ "));
	prompt = ft_strjoin(tmp, "$ ");
	free(tmp);
	if (!prompt)
		return (ft_strdup("minishell$ "));
	return (prompt);
}

void	cleanup_and_exit(t_shell *shell)
{
	rl_clear_history();
	free_array(shell->env);
}

void	cleanup_child_process(t_parser *parser, t_shell *shell)
{
	free_parser(parser);
	if (shell->current_tokens)
	{
		free_tokens(shell->current_tokens);
		shell->current_tokens = NULL;
	}
	if (shell->current_line)
	{
		free(shell->current_line);
		shell->current_line = NULL;
	}
	cleanup_and_exit(shell);
}
