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

t_env	*creat_env_list(char **env)
{
	t_env	*head;
	t_env	*node;
	int		equal;

	head = malloc(sizeof(t_env));
	malloc_err(!head, "env list");
	node = head;
	while (*env)
	{
		equal = ft_strchr(*env, '=') - *env;
		node->line = ft_strdup(*env);
		node->key = ft_substr(*env, 0, equal);
		node->value = ft_substr(*env, equal + 1, ft_strlen(*env) - equal);
		malloc_err(!node->line || !node->key || !node->line, "env list");
		if (!*(env + 1))
			node->next = NULL;
		else
		{
			node->next = malloc(sizeof(t_env));
			malloc_err(!node->next, "env list");
		}
		node = node->next;
		env++;
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
		env(vars, 1);
	else if (!ft_strcmp(cmd, "echo"))
		echo(vars);
	else if (!ft_strcmp(cmd, "export"))
		export(vars);
	else if (!ft_strcmp(cmd, "unset"))
		unset(vars);
	else if (!ft_strcmp(cmd, "exit"))
		exit_prog(vars);
	return (0);
}

int	change_spaces(char **str, int *i, char c)
{
	while ((*str)[++*i] && (*str)[*i] != c)
		if ((*str)[*i] == 32)
			(*str)[*i] = 1;
	if ((*str)[*i] == c)
		return (1);
	return (0);
}

void	quotes_handler(char **str)
{
	int	i;
	int	d_count;
	int	s_count;

	i = -1;
	d_count = 0;
	s_count = 0;
	while ((*str)[++i])
	{
		if ((*str)[i] == '\"')
		{
			d_count++;
			if (change_spaces(str, &i, '\"'))
				d_count++;
			else
				break ;
		}
		else if ((*str)[i] == '\'')
		{
			s_count++;
			if (change_spaces(str, &i, '\''))
				s_count++;
			else
				break ;
		}
	}
	// if (d_count % 2 != 0)
	// 	// 
	// if (s_count % 2 != 0)
	// 	// 
}

void	restore_spaces(t_vars *vars)
{
	int	i;
	int	j;

	i = -1;
	while (vars->cmd[++i])
	{
		j = -1;
		while (vars->cmd[i][++j])
		{
			if (vars->cmd[i][j] == 1)
				vars->cmd[i][j] = 32;
		}
	}
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
	vars.env = creat_env_list(env);
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
		quotes_handler(&input_str);
		vars.cmd = ft_split(input_str, ' ');
		free(input_str);
		malloc_err(!vars.cmd, "creating cmd list");
		restore_spaces(&vars);
		if (!*vars.cmd)
			continue ;
		if (check_builtins(tolower_str(&*vars.cmd), &vars))
			printf("minishell: %s: command not found\n", *vars.cmd); //err_mes
		split_free(vars.cmd);
	}
	return (0);
}
