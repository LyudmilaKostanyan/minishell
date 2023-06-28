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

int	check_builtins(t_vars *vars, char **cmd)
{
	if (!ft_strcmp(*cmd, "pwd"))
		pwd(vars);
	else if (!ft_strcmp(*cmd, "cd"))
		cd(vars, cmd);
	else if (!ft_strcmp(*cmd, "env"))
		env(vars, 1);
	else if (!ft_strcmp(*cmd, "echo"))
		echo(vars, cmd);
	else if (!ft_strcmp(*cmd, "export"))
		export(vars, cmd);
	else if (!ft_strcmp(*cmd, "unset"))
		unset(vars, cmd);
	else if (!ft_strcmp(*cmd, "exit"))
		exit_prog(cmd);
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

void	check_equal(t_vars	*vars, char **cmd)
{
	int			i;
	long long	equal;
	char		*key;

	i = -1;
	while (cmd[++i])
	{
		equal = ft_strchr(cmd[i], '=') - cmd[i];
		key = ft_substr(cmd[i], 0, equal);
		malloc_err(!key, cmd[0]);
		if (equal >= 0 && !ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e')
			&& !check_set(vars->env, cmd[i], key, equal)
			&& !check_set(vars->set, cmd[i], key, equal))
			creat_env_var(&vars->set, cmd[i], key, equal);
		free(key);
	}
}

void	str_changes(t_vars *vars, t_cmds *cmds, char *input_str)
{
	char	*for_split;
	int		j;

	quotes_handler(vars, &input_str);
	for_split = rm_quotes(vars, input_str);
	if (!for_split)
		cmds->cmd = ft_split(input_str, ' ');
	else
		cmds->cmd = ft_split(for_split, ' ');
	malloc_err(!*cmds->cmd, "creating cmd list");
	j = -1;
	while (cmds->cmd[++j])
		restore_spaces(&cmds->cmd[j]);
	restore_spaces(&input_str);
	add_history(input_str);
	free(for_split);
}

void	pipes(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count - 1)
	{
		printf("asd\n");
		if (pipe(cmds[i]->pipe) == -1)
		{
			while (--i >= 0)
			{
				close(cmds[i]->pipe[0]);
				close(cmds[i]->pipe[1]);
			}
			stop_program(1, "", "creat pipes", vars->exit_stat);
		}
	}
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;
	char	*input_str;
	t_cmds	*cmds;

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
		char	**pipe_splt = ft_split(input_str, '|');
		malloc_err(!pipe_splt, "split cmds");
		int	count = split_size(pipe_splt);
		cmds = malloc((sizeof(t_cmds) * count) + 1);
		malloc_err(!cmds, "creat cmds");
		int	i = -1;
		while (++i < count)
		{
			str_changes(&vars, &cmds[i], pipe_splt[i]);
			// if (!*vars.cmd)								//?????????
			// 	continue ;
		}
		// i = -1;
		// while (++i < count - 1)
		// {
		// 	cmds[i].pipe = malloc(sizeof(int) * 2);
		// 	malloc_err(!cmds[i].pipe, "creat pipes");
		// }
		// // system("leaks minishell");
		// pipes(&vars, &cmds, count);
		i = -1;
		while (++i < count)
		{
			check_equal(&vars, cmds[i].cmd);
			tolower_str(*cmds[i].cmd);
			check_builtins(&vars, cmds[i].cmd);
		}
		i = -1;
		while (++i < count)
			split_free(cmds[i].cmd);
		split_free(pipe_splt);
		free(cmds);
		free(input_str);
	}
	return (0);
}
