#include "minishell.h"
#include <libft.h>

int	split_size(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		;
	return (i);
}

void	split_free(char **split)
{
	int	i;

	i = split_size(split) - 1;
	while (i > -1)
	{
		free(split[i]);
		i--;
	}
	free(split);
	split = NULL;
}

char	*tolower_str(char **str)
{
	int	i;

	i = -1;
	while ((*str)[++i])
		if ((*str)[i] >= 65 && (*str)[i] <= 90)
			(*str)[i] = (*str)[i] + 32;
	return (*str);
}

int	check_builtins(char *cmd, char *arg)
{
	char *str = malloc(500); //?????????????????????

	if (!ft_strcmp(cmd, "pwd"))
		printf("%s\n", getcwd(str, 500)); //getenv("PWD"));
	else if (!ft_strcmp(cmd, "cd"))
		if (chdir(arg) != 0)
			perror("cd: ");
	// else if (!ft_strcmp(cmd, "cd"))
	free(str); //?????????????????
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;
	(void)env;

	pid_t	main_pid;
	char	*input_str;
	char	**cmd_arg;

	if (argc != 1)
	{
		perror("No such file or directory");
		exit(0);
	}
	main_pid = fork();
	if (!main_pid)
	{
		input_str = readline("minishell$ ");
		while (input_str)
		{
			add_history(input_str);
			cmd_arg = ft_split(input_str, ' ');
			if (cmd_arg == NULL)
			{
				perror("Malloc error");
				exit(0);
			}
			check_builtins(tolower_str(&*cmd_arg), cmd_arg[1]);
			free(input_str);
			rl_redisplay();
			split_free(cmd_arg);
			input_str = readline("minishell$ ");
		}
	}
	else
		wait(NULL);
	return (0);
}