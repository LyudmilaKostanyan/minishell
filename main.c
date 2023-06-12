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

void	stop_program(int condition, char *str, int exit_stat)
{
	if (condition)
	{
		perror(str);
		exit(exit_stat);
	}
}

t_env	*creat_env_list(char **env, int exit_stat)
{
	t_env	*head;
	t_env	*node;
	int		i;

	i = -1;
	head = malloc(sizeof(t_env));
	stop_program(!head, "Malloc error", exit_stat);
	node = head;
	while (env[++i])
	{
		if (!i)
			node->prev = NULL;
		node->line = ft_strdup(env[i]);
		stop_program(!node->line, "Malloc error", exit_stat);
		if (!env[i + 1])
			node->next = NULL;
		else
		{
			node->next = malloc(sizeof(t_env));
			stop_program(!node->next, "Malloc error", exit_stat);
			node->next->prev = node;
		}
		node = node->next;
	}
	return (head);
}

char	*tolower_str(char **str)
{
	int	i;

	i = -1;
	while (*str && (*str)[++i])
		if ((*str)[i] >= 65 && (*str)[i] <= 90)
			(*str)[i] = (*str)[i] + 32;
	return (*str);
}

int	check_builtins(char *cmd, char **args, t_vars *vars)
{
	char	*pwd;
	int		i;

	if (!ft_strcmp(cmd, "pwd"))
	{
		pwd = getcwd(NULL, 0);
		stop_program(!pwd, "Malloc error", vars->exit_stat);
		printf("%s\n", pwd);
		free(pwd);
	}
	else if (!ft_strcmp(cmd, "cd"))
	{
		if (chdir(args[1]) != 0)
			perror("cd: ");
		else
			vars->exit_stat++;
	}
	else if (!ft_strcmp(cmd, "echo"))
	{
		if (ft_strcmp(args[1], "-n"))
			i = 0;
		else
			i = 1;
		while (args[++i])
		{
			printf("%s", args[i]);
			if (args[i + 1])
				printf(" ");
		}
		if (ft_strcmp(args[1], "-n"))
			printf("\n");
	}
	else if (!ft_strcmp(cmd, "exit"))
		exit(1);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;
	char	*input_str;

	(void)argv;
	if (argc != 1)
	{
		perror("No such file or directory");
		exit(0);
	}
	vars.exit_stat = 0;
	vars.env = creat_env_list(env, vars.exit_stat);
	while (1)
	{
		input_str = readline("minishell$ ");
		if (!input_str)
		{
			printf("exit"); // ^D ??????????
			exit(vars.exit_stat);
		}
		else if (!*input_str)
			continue ;
		add_history(input_str);
		vars.cmd_arg = ft_split(input_str, ' ');
		free(input_str);
		stop_program(!vars.cmd_arg, "Malloc error", vars.exit_stat);
		if (!*vars.cmd_arg)
			continue ;
		if (check_builtins(tolower_str(&*vars.cmd_arg), vars.cmd_arg, &vars))
			printf("minishell: %s: command not found\n", *vars.cmd_arg);
		split_free(vars.cmd_arg);
	}
	return (0);
}
