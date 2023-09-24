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
	malloc_err(!pwd, "pwd", vars);
	printf("%s\n", pwd);
	free(pwd);
	g_exit_status = 0;
}

void	change_oldpwd(t_vars *vars, char **cmd)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	malloc_err(!old_pwd, *cmd, vars);
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
		&& (err_mes(access(cmd[1], F_OK) == -1,
				join_err(vars, *cmd, cmd[1]), CD_ERR, vars)))
		return (0);
	if (!ft_strcmp(cmd[1], "-"))
	{
		tmp = find_key(*vars, "OLDPWD");
		if (err_mes(!tmp, join_err(vars, *cmd, NULL), "OLDPWD not set", vars))
			return (0);
		pwd = ft_strdup(tmp->value);
		malloc_err(!pwd, *cmd, vars);
		if (tmp)
		{
			change_oldpwd(vars, cmd);
			err_mes(chdir(pwd) == -1, join_err(vars, "cd", pwd), PD, vars);
		}
		free(pwd);
		return (0);
	}
	else
		change_oldpwd(vars, cmd);
	return (1);
}

int	cd_help(t_vars *vars, char **cmd)
{
	t_env	*tmp;

	tmp = find_key(*vars, "HOME");
	if (err_mes(!tmp, join_err(vars, *cmd, NULL), "HOME not set", vars))
		return (0);
	if (tmp)
		err_mes(chdir(tmp->value) == -1,
			join_err(vars, "cd", tmp->value), PD, vars);
	if (!check_set(vars, vars->env, tmp->value, "PWD"))
		creat_env_var(vars, &vars->env, tmp->value, "PWD");
	return (1);
}

void	cd(t_vars *vars, char **cmd)
{
	char	*pwd;

	if (!cd_check_args(vars, cmd))
		return ;
	vars->equal = 0;
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		if (!cd_help(vars, cmd))
			return ;
	}
	else
	{
		err_mes(chdir(cmd[1]) == -1, join_err(vars, "cd", cmd[1]), PD, vars);
		pwd = getcwd(NULL, 0);
		malloc_err(!pwd, cmd[0], vars);
		if (!check_set(vars, vars->env, pwd, "PWD"))
			creat_env_var(vars, &vars->env, pwd, "PWD");
		free(pwd);
	}
	g_exit_status = 0;
}
