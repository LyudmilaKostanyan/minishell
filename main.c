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

void	wait_stop(char **input_str, char c, int *count)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		tmp = ft_strjoin(*input_str, "\n");
		malloc_err(!tmp, "reading input");
		free(*input_str);
		*input_str = tmp;
		line = readline("> ");
		if (!line)
		{
			printf("bash: unexpected EOF while looking for matching `\"\'\nbash: syntax error: unexpected end of file\n"); // ^D ??????????
			break ;
		}
		
		tmp = ft_strjoin(*input_str, line);
		malloc_err(!tmp, "reading input");
		free(*input_str);
		*input_str = tmp;
		if (ft_strchr(line, c))
		{
			(*count)++;
			free(line);
			break ;
		}
		free(line);
	}
}

int	change_spaces(char **input_str, int *i, char c, int *count)
{
	while ((*input_str)[++*i] && (*input_str)[*i] != c)
	{
		if (c == '\"')
		{
			if ((*input_str)[*i] == '\"')
				(*count)++;
		}
		else if (c == '\'')
			if ((*input_str)[*i] == '\'')
				(*count)++;
		if ((*input_str)[*i] == 32)
			(*input_str)[*i] = 1;						//other func!!!!!!
	}
	if ((*input_str)[*i] == c)
		return (1);
	if (!(*input_str)[*i] && *count % 2 != 0)
		wait_stop(input_str, c, count);
	return (0);
}

void	quotes_handler(char **input_str)
{
	int	i;
	int	d_count;
	int	s_count;

	i = -1;
	d_count = 0;
	s_count = 0;
	while ((*input_str)[++i])
	{
		if ((*input_str)[i] == '\"')
		{
			d_count++;
			if (!change_spaces(input_str, &i, '\"', &d_count))
				break ;
		}
		else if ((*input_str)[i] == '\'')
		{
			s_count++;
			if (!change_spaces(input_str, &i, '\'', &s_count))
				break ;
		}
	}
}

char	*rm_quotes()
{
	
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
	char	*for_split;

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
		quotes_handler(&input_str);
		vars.cmd = ft_split(input_str, ' ');
		malloc_err(!vars.cmd, "creating cmd list");
		restore_spaces(&vars);
		add_history(input_str);
		free(input_str);
		if (!*vars.cmd)
			continue ;
		if (check_builtins(tolower_str(&*vars.cmd), &vars))
			printf("minishell: %s: command not found\n", *vars.cmd); //err_mes
		split_free(vars.cmd);
	}
	return (0);
}
