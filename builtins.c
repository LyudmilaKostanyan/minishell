/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:27 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/14 19:17:29 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pwd(t_vars *vars)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	malloc_err(!pwd, "pwd");
	printf("%s\n", pwd);
	free(pwd);
	vars->exit_stat = 0;
}

void	cd(t_vars *vars, char **cmd)
{
	char	*pwd;
	char	*old_pwd;
	t_env	*tmp;

	vars->exit_stat = 1;
	if (!cmd[1] || !ft_strcmp(cmd[1], "~"))
	{
		tmp = find_key(*vars, "HOME");
		if (!tmp)
			chdir("/");			//change
		else
			chdir(tmp->value);
	}
	else if (!ft_strcmp(cmd[1], "-"))
	{
		tmp = find_key(*vars, "OLDPWD");
		err_mes(!tmp, vars, cmd, "OLDPWD not set");
		if (tmp)
			chdir(tmp->value);
	}
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
		tmp = vars->env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->line, "PWD=", 4))
			{
				free(tmp->line);
				tmp->line = ft_strjoin("PWD=", pwd);
				malloc_err(!tmp->line, cmd[0]);
				break ;
			}
			tmp = tmp->next;
		}
		free(pwd);
		vars->exit_stat = 0;
	}
}

void	echo(t_vars *vars, char **cmd)
{
	int	i;
	int	j;
	int	cond;

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
	vars->exit_stat = 0;
	while (cmd[++i])
	{
		if (!ft_isdigit(*cmd[i]) && ft_isalnum_str(cmd[i], 'u'))
			find_node(vars, cmd[i]);
		else
			err_mes(1, vars, cmd, cmd[i]);
	}
}

void	exit_prog(char **cmd)
{
	long long	exit_code;

	if (cmd[1])
	{
		exit_code = ft_atoll(cmd[1]);
		exit(exit_code % 256);
	}
	else
		exit(0);
}
