#include "minishell.h"

extern char **environ;

char	**copy_env(char **env)
{
	char	**env_copy;
	int		i;
	int		count;

	count = 0;
	while (env[count])
		count++;
	env_copy = malloc(sizeof(char *) * (count + 1));
	if (!env_copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env_copy[i] = ft_strdup(env[i]);
		i++;
	}
	env_copy[i] = NULL;
	return (env_copy);
}

char	*get_env_var(char *name, char **env)
{
	int	i;
	int	len;

	if (!env)
		env = environ;
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	set_env_var(char *name, char *value, char **env)
{
	int		i;
	int		len;
	char	*new_var;

	(void)env;
	len = ft_strlen(name) + ft_strlen(value) + 2;
	new_var = malloc(len);
	if (!new_var)
		return;
	ft_strlcpy(new_var, name, len);
	ft_strlcat(new_var, "=", len);
	ft_strlcat(new_var, value, len);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, ft_strlen(name)) == 0 && 
			environ[i][ft_strlen(name)] == '=')
		{
			free(environ[i]);
			environ[i] = new_var;
			return;
		}
		i++;
	}
	// Don't realloc environ - just print error
	write(STDERR_FILENO, "minishell: export: cannot modify environment\n",
		ft_strlen("minishell: export: cannot modify environment\n"));
	free(new_var);
}

void	unset_env_var(char *name, char **env)
{
	int	i;
	int	len;

	(void)env;
	len = ft_strlen(name);
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
		{
			free(environ[i]);
			while (environ[i])
			{
				environ[i] = environ[i + 1];
				i++;
			}
			return;
		}
		i++;
	}
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	sigint_handler(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	// Simple prompt display without readline functions
	write(STDOUT_FILENO, "minishell$ ", ft_strlen("minishell$ "));
}

void	sigquit_handler(int sig)
{
	g_signal_received = sig;
}

void	print_error(char *msg)
{
	printf("minishell: %s\n", msg);
}
