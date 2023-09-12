/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:27 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 19:25:41 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_vars *vars)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	malloc_err(!pwd, "pwd", vars->true_env);///
	printf("%s\n", pwd);
	free(pwd);
	g_exit_status = 0;
}

void	change_oldpwd(t_vars *vars, char **cmd)
{
	char	*old_pwd;
	old_pwd = getcwd(NULL, 0);
	malloc_err(!old_pwd, *cmd, vars->true_env);///
	vars->equal = 0;
	if (!check_set(vars, vars->env, old_pwd))///
		creat_env_var(vars, &vars->env, old_pwd, "OLDPWD");///
	free(old_pwd);
}

void	cd(t_vars *vars, char **cmd)
{
	char	*pwd;
	t_env	*tmp;

	if (cmd[1] && *cmd[1] != '~' && *cmd[1] != '-' &&
		(err_mes(access(cmd[1], F_OK) == -1, *cmd, cmd[1], CD_ERR)
		|| err_mes(cmd[2] != NULL, *cmd, cmd[1], TMA)))
		return ;
	if (!ft_strcmp(cmd[1], "-"))
	{
		tmp = find_key(*vars, "OLDPWD");
		err_mes(!tmp, *cmd, NULL, "OLDPWD not set");
		pwd = ft_strdup(tmp->value);
		malloc_err(!pwd, *cmd, vars->true_env);///
		if (tmp)
		{
			change_oldpwd(vars, cmd);
			chdir(pwd);
		}
		free(pwd);
	}
	else
		change_oldpwd(vars, cmd);
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		tmp = find_key(*vars, "HOME");
		err_mes(!tmp, *cmd, NULL, "HOME not set");
		if (tmp)
			chdir(tmp->value);
		vars->equal = 0;
		if (!check_set(vars, vars->env, tmp->value))///
			creat_env_var(vars, &vars->env, tmp->value, "PWD";///
	}
	else
	{
		chdir(cmd[1]);
		pwd = getcwd(NULL, 0);
		malloc_err(!pwd, cmd[0], vars->true_env);///
		if (!check_set(vars->env, pwd, "PWD", 0))
			creat_env_var(&vars->env, pwd, "PWD", 0);
		free(pwd);
	}
	g_exit_status = 0;
}

void	echo(char **cmd)
{
	int		i;
	int		j;
	int		cond;
	char	*start;

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
	cond = i;
	while (cmd[i])
	{
		j = -1;
		start = ft_strchr(cmd[i], '$');
		if (!start || ft_strncmp(start, "$?", 2))
			printf("%s", cmd[i]);
		else
		{
			while (cmd[i] + ++j != start)
				printf("%c", cmd[i][j]);
			printf("%d", g_exit_status);
			printf("%s", cmd[i] + j + 2);
		}
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (cond == 1)
		printf("\n");
	g_exit_status = 0;
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
	g_exit_status = 0;
}

int	free_node(t_env **list, t_env *env, char *cmd)
{
	t_env	*tmp;

	if (!ft_strcmp(env->key, cmd))
	{
		*list = env->next;
		free(env->line);
		free(env->key);
		free(env->value);
		free(env);
	}
	else if (env->next && !ft_strcmp(env->next->key, cmd))
	{
		tmp = env->next->next;
		free(env->next->line);
		free(env->next->key);
		free(env->next->value);
		free(env->next);
		env->next = tmp;
	}
	else
		return (0);
	return (1);
}

void	find_node(t_vars *vars, char *cmd)
{
	t_env	*env;

	env = vars->env;
	while (env)
	{
		if (free_node(&vars->env, env, cmd))
			break ;
		env = env->next;
	}
	env = vars->set;
	while (env)
	{
		if (free_node(&vars->set, env, cmd))
			break ;
		env = env->next;
	}
}

void	unset(t_vars *vars, char **cmd)
{
	int		i;

	i = 0;
	g_exit_status = 0;
	while (cmd[++i])
	{
		if (!ft_isdigit(*cmd[i]) && ft_isalnum_str(cmd[i], 'u'))
			find_node(vars, cmd[i]);
		else
			err_mes(1, *cmd, cmd[i], E_U_ERR);
	}
}

void	exit_prog(t_vars *vars, char **cmd)
{
	long long	exit_code;

	if (err_mes(cmd[1] && cmd[2] != NULL, *cmd, NULL, TMA))		// == NULL???
		return ;
	if (cmd[1])
	{
		exit_code = ft_atoll(vars, cmd[1]); ///
		exit(exit_code % 256);
	}
	else
		exit(0);
}
