/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:50:44 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/15 18:50:49 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_len(t_env *env)
{
	int		len;
	t_env	*head;

	len = 0;
	head = env;
	while (env)
	{
		len++;
		env = env->next;
	}
	env = head;
	return (len);
}

void	env_to_str(t_vars *vars)
{
	int		len;
	int		i;
	t_env	*head;

	len = env_len(vars->env);
	vars->env_var = malloc(sizeof(char *) * (len + 1));
	i = -1;
	head = vars->env;
	while (++i < len)
	{
		vars->env_var[i] = vars->env->line;
		vars->env = vars->env->next;
	}
	vars->env_var[i] = NULL;
	vars->env = head;
}

void	creating_exec_path(t_vars *vars)
{
	int		i;
	char	*tmp;

	vars->paths = NULL;
	env_to_str(vars);
	i = -1;
	while (vars->env_var[++i])
		if (!ft_strncmp(vars->env_var[i], "PATH=", 5))
			break ;
	if (vars->env_var[i])
	{
		tmp = ft_substr(vars->env_var[i], 5, ft_strlen(vars->env_var[i]) - 5);
		vars->paths = ft_split(tmp, ':');
		free(tmp);
	}
}

int	path_check(t_vars *vars, t_cmds **cmds, char *cmd, int i)
{
	char	*tmp;
	int		j;

	if (!cmd)
		return (1);
	creating_exec_path(vars);
	if (access(cmd, X_OK) != -1)
	{
		(*cmds)[i].ex_cmd = ft_strdup(cmd);
		return (1);
	}
	j = 0;
	while (vars->paths && vars->paths[j])
	{
		tmp = ft_strjoin(vars->paths[j], "/");
		malloc_err(!tmp, "path_check", vars->true_env);
		(*cmds)[i].ex_cmd = ft_strjoin(tmp, cmd);
		malloc_err(!(*cmds)[i].ex_cmd, "path_check", vars->true_env);
		free(tmp);
		if (access((*cmds)[i].ex_cmd, X_OK) != -1)
			return (1);
		j++;
		free((*cmds)[i].ex_cmd);
	}
	err_mes(1, cmd, NULL, "command not found");
	// write(vars->fd_out, "command not found\n", 18);
	exit(127);
	return (0);
}
