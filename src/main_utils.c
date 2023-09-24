/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 20:13:52 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 20:13:52 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*creat_env_list(t_vars *vars, char **env)
{
	t_env	*head;
	t_env	*node;
	int		equal;

	head = malloc(sizeof(t_env));
	malloc_err(!head, "env list", vars);
	node = head;
	while (*env)
	{
		equal = ft_strchr(*env, '=') - *env;
		node->line = ft_strdup(*env);
		node->key = ft_substr(*env, 0, equal);
		node->value = ft_substr(*env, equal + 1, ft_strlen(*env) - equal);
		malloc_err(!node->line || !node->key, "env list", vars);
		if (!*(env + 1))
			node->next = NULL;
		else
		{
			node->next = malloc(sizeof(t_env));
			malloc_err(!node->next, "env list", vars);
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
		echo(cmd);
	else if (!ft_strcmp(*cmd, "export"))
		export(vars, cmd);
	else if (!ft_strcmp(*cmd, "unset"))
		unset(vars, cmd);
	else if (!ft_strcmp(*cmd, "exit"))
		exit_prog(vars, cmd);
	else
		return (0);
	return (1);
}

int	check_equal(t_vars *vars, char **cmd)
{
	int	i;
	int	cond;

	i = -1;
	cond = 0;
	while (cmd[++i])
	{
		vars->equal = ft_strchr(cmd[i], '=') - cmd[i];
		vars->key = ft_substr(cmd[i], 0, vars->equal);
		malloc_err(!vars->key, cmd[0], vars);
		if (vars->equal >= 0 && !ft_isdigit(*vars->key) && *vars->key
			&& ft_isalnum_str(vars->key, 'e') && ++cond
			&& !check_set(vars, vars->env, cmd[i], vars->key)
			&& !check_set(vars, vars->set, cmd[i], vars->key))
			creat_env_var(vars, &vars->set, cmd[i], vars->key);
		free(vars->key);
	}
	return (cond);
}

void	free_cmds(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	dup2(vars->fd_in, 0);
	dup2(vars->fd_out, 1);
	i = -1;
	while (++i < count)
	{
		split_free((*cmds)[i].cmd);
		free((*cmds)[i].red_in);
		free((*cmds)[i].red_out);
		if (i != count - 1)
			free((*cmds)[i].pipe);
	}
	free(*cmds);
}

int	check_redirection(t_cmds *cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (err_mes(cmds[i].in_stat == -1
				|| cmds[i].out_stat == -1, NULL, NULL, "syntax error"))
		{
			g_exit_status = 258;
			return (0);
		}
	}
	return (1);
}
