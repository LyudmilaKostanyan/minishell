/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:27 by lykostan          #+#    #+#             */
/*   Updated: 2023/07/31 23:56:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_help(t_vars *vars, char **cmd)
{
	t_env	*tmp;

	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))		//diferent
	{
		tmp = find_key(*vars, "HOME");
		err_mes(!tmp, vars, cmd, "HOME not set");
		if (tmp)
			chdir(tmp->value);
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		tmp = find_key(*vars, "OLDPWD");
		err_mes(!tmp, vars, cmd, "OLDPWD not set");		//?????????
		if (tmp)
			chdir(tmp->value);
	}
}

void	cd(t_vars *vars, char **cmd)
{
	char	*pwd;
	char	*old_pwd;

	vars->exit_stat = 1;
	cd_help(vars, cmd);
	old_pwd = getcwd(NULL, 0);
	malloc_err(!old_pwd, cmd[0]);
	if (!check_set(vars->env, old_pwd, "OLDPWD", 0))
		creat_env_var(&vars->env, old_pwd, "OLDPWD", 0);
	free(old_pwd);
	if (cmd[1] && ft_strcmp(cmd[1], "~") && ft_strcmp(cmd[1], "-")
		&& !err_mes(cmd[2] != NULL, vars, cmd, cmd[1])
		&& !err_mes(chdir(cmd[1]), vars, cmd, cmd[1]))
	{
		pwd = getcwd(NULL, 0);
		malloc_err(!pwd, cmd[0]);
		if (!check_set(vars->env, pwd, "PWD", 0))
			creat_env_var(&vars->env, pwd, "PWD", 0);
		free(pwd);
		vars->exit_stat = 0;
	}
}

static void	echo_help(t_vars *vars, char **cmd, int i)
{
	int	cond;

	cond = i;
	while (cmd[i])
	{
		if (ft_strcmp(cmd[i], "$?"))
			printf("%s", cmd[i]);
		else
			printf("%d", vars->exit_stat);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (cond == 1)
		printf("\n");
	vars->exit_stat = 0;
}

void	echo(t_vars *vars, char **cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[++i])
	{
		j = 0;
		if (cmd[i][j] != '-')
			break ;
		while (cmd[i][++j])
			if (cmd[i][j] != 'n')
				break ;
		if (j != (int)ft_strlen(cmd[i]))
			break ;
	}
	echo_help(vars, cmd, i);
}

void	env(t_vars *vars, int cmd)
{
	t_env	*tmp;

	tmp = vars->env;
	while (tmp)
	{
		if (cmd)
			printf("%s\n", tmp->line);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	vars->exit_stat = 0;
}
