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
	else
		return (0);
	return (1);
}

void	restore_spaces(char **str)
{
	int	i;

	i = -1;
	while ((*str)[++i])
		if ((*str)[i] == 1)
			(*str)[i] = 32;
}

void	check_equal(t_vars	*vars)
{
	int			i;
	long long	equal;
	char		*key;

	i = -1;
	while (vars->cmd[++i])
	{
		equal = ft_strchr(vars->cmd[i], '=') - vars->cmd[i];
		key = ft_substr(vars->cmd[i], 0, equal);
		malloc_err(!key, vars->cmd[0]);
		if (equal >= 0 && !ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e')
			&& !check_env_vars(vars->env, vars->cmd[i], key, equal)
			&& !check_env_vars(vars->env_vars, vars->cmd[i], key, equal))
			creat_env_var(&vars->env_vars, vars->cmd[i], key, equal);
		free(key);
	}
}

void	str_changes(t_vars *vars, char **input_str)
{
	char	*for_split;
	int		i;

	quotes_handler(vars, input_str);
	for_split = rm_quotes(vars, *input_str);
	if (!for_split)
		vars->cmd = ft_split(*input_str, ' ');
	else
		vars->cmd = ft_split(for_split, ' ');
	malloc_err(!vars->cmd, "creating cmd list");
	i = -1;
	while (vars->cmd[++i])
		restore_spaces(&vars->cmd[i]);
	restore_spaces(input_str);
	add_history(*input_str);
	free(*input_str);
	free(for_split);
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
		str_changes(&vars, &input_str);
		if (!*vars.cmd)
			continue ;
		check_equal(&vars);
		check_builtins(tolower_str(*vars.cmd), &vars);
		split_free(vars.cmd);
	}
	return (0);
}
