/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:51 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/14 19:17:54 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

t_env	*creat_env_list(char **env, int exit_stat)
{
	t_env	*head;
	t_env	*node;
	int		i;

	// key-value create
	i = -1;
	head = malloc(sizeof(t_env));
	stop_program(!head, "history", MALL_ERR, exit_stat);
	node = head;
	while (env[++i])
	{
		// if (!i)
		// 	node->prev = NULL;
		node->line = ft_strdup(env[i]);
		stop_program(!node->line, "history", MALL_ERR, exit_stat);
		if (!env[i + 1])
			node->next = NULL;
		else
		{
			node->next = malloc(sizeof(t_env));
			stop_program(!node->next, "history", MALL_ERR, exit_stat);
			// node->next->prev = node;
		}
		node = node->next;
	}
	return (head);
}

int	check_builtins(char *cmd, t_vars *vars)
{
	if (!ft_strcmp(cmd, "pwd"))
		pwd(vars);
	else if (!ft_strcmp(cmd, "cd"))
		cd(vars);
	else if (!ft_strcmp(cmd, "env"))
		env(vars);
	else if (!ft_strcmp(cmd, "echo"))
		echo(vars);
	else if (!ft_strcmp(cmd, "export"))
		export(vars);
	else if (!ft_strcmp(cmd, "unset"))
		unset(vars);
	else if (!ft_strcmp(cmd, "exit"))
		exit_prog(vars);
	// echo without ""!!!!!!!!
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
		input_str = readline("\e[34mminishell$ \e[0m");
		if (!input_str)
		{
			printf("exit\n"); // ^D ??????????
			exit(vars.exit_stat);
		}
		else if (!*input_str)
			continue ;
		add_history(input_str);
		vars.cmd = ft_split(input_str, ' ');
		free(input_str);
		stop_program(!vars.cmd, "history", MALL_ERR, vars.exit_stat);
		if (!*vars.cmd)
			continue ;
		if (check_builtins(tolower_str(&*vars.cmd), &vars))
			printf("minishell: %s: command not found\n", *vars.cmd);
		split_free(vars.cmd);
	}
	system("leaks minishell");
	return (0);
}
