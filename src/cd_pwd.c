/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:48:50 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/15 18:48:52 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_vars *vars)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	malloc_err(!pwd, "pwd", vars->true_env);
	printf("%s\n", pwd);
	free(pwd);
	g_exit_status = 0;
}

void	change_oldpwd(t_vars *vars, char **cmd)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	malloc_err(!old_pwd, *cmd, vars->true_env);
	vars->equal = 0;
	vars->key = "OLDPWD";
	if (!check_set(vars, vars->env, old_pwd, "OLDPWD"))
		creat_env_var(vars, &vars->env, old_pwd, "OLDPWD");
	free(old_pwd);
}

int	cd_check_args(t_vars *vars, char **cmd)
{
	char	*pwd;
	t_env	*tmp;

	if (cmd[1] && *cmd[1] != '~' && *cmd[1] != '-'
		&& (err_mes(access(cmd[1], F_OK) == -1, *cmd, cmd[1], CD_ERR)
			|| err_mes(cmd[2] != NULL, *cmd, cmd[1], TMA)))
		return (0);
	if (!ft_strcmp(cmd[1], "-"))
	{
		tmp = find_key(*vars, "OLDPWD");
		err_mes(!tmp, *cmd, NULL, "OLDPWD not set");
		pwd = ft_strdup(tmp->value);
		malloc_err(!pwd, *cmd, vars->true_env);
		if (tmp)
		{
			change_oldpwd(vars, cmd);
			err_mes(chdir(pwd) == -1, "cd", pwd, PD);
		}
		free(pwd);
	}
	else
		change_oldpwd(vars, cmd);
	return (1);
}

void	cd(t_vars *vars, char **cmd)
{
	char	*pwd;
	t_env	*tmp;

	if (!cd_check_args(vars, cmd))
		return ;
	vars->equal = 0;
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		tmp = find_key(*vars, "HOME");
		err_mes(!tmp, *cmd, NULL, "HOME not set");
		if (tmp)
			err_mes(chdir(tmp->value) == -1, "cd", tmp->value, PD);
		if (!check_set(vars, vars->env, tmp->value, "PWD"))
			creat_env_var(vars, &vars->env, tmp->value, "PWD");
	}
	else
	{
		err_mes(chdir(cmd[1]) == -1, "cd", cmd[1], PD);
		pwd = getcwd(NULL, 0);
		malloc_err(!pwd, cmd[0], vars->true_env);
		if (!check_set(vars, vars->env, pwd, "PWD"))
			creat_env_var(vars, &vars->env, pwd, "PWD");
		free(pwd);
	}
	g_exit_status = 0;
}
