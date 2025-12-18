#include "minishell.h"

int	g_signal_received = 0;
t_history	g_history;

int	main(int argc, char **argv, char **env)
{
	char	**env_copy;

	env_copy = copy_env(env);
	init_shell();
	
	// Display welcome message if argument provided
	if (argc > 1)
		display_welcome(argv[1]);
	
	run_shell();
	cleanup_shell();
	free_array(env_copy);
	return (0);
}

void	init_shell(void)
{
	setup_signals();
	init_history();
}

void	run_shell(void)
{
	char	*input;
	t_token	*tokens;
	t_parser	*parser;

	while (1)
	{
		input = enhanced_readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (ft_strlen(input) > 0)
		{
			add_to_history(input);
			tokens = tokenize(input);
			if (tokens)
			{
				parser = parse_tokens(tokens);
				if (parser)
				{
					execute_commands(parser);
					free_parser(parser);
				}
				free_tokens(tokens);
			}
		}
		free(input);
	}
}

void	cleanup_shell(void)
{
	clear_history();
}

void	execute_commands(t_parser *parser)
{
	int	i;

	if (parser->pipe_count > 0)
	{
		execute_pipeline(parser);
		return;
	}
	i = 0;
	while (i < parser->cmd_count)
	{
		execute_command(&parser->cmds[i], NULL);
		i++;
	}
}

void	execute_pipeline(t_parser *parser)
{
	int	i;
	int	pid;
	int	status;

	setup_pipes(parser);
	i = 0;
	while (i < parser->cmd_count)
	{
		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
				dup2(parser->cmds[i].pipe_in, STDIN_FILENO);
			if (i < parser->cmd_count - 1)
				dup2(parser->cmds[i].pipe_out, STDOUT_FILENO);
			close_pipes(parser);
			execute_command(&parser->cmds[i], NULL);
			exit(0);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
		}
		i++;
	}
	close_pipes(parser);
}

void	display_welcome(char *message)
{
	printf("%s%s%s\n", BLUE, message, RESET);
}

void	display_prompt(void)
{
	printf("minishell$ ");
	fflush(stdout);
}
