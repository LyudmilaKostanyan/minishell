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
	old_pwd = getcwd(NULL, 0);
	malloc_err(!old_pwd, cmd[0]);
	if (!check_set(vars->env, old_pwd, "OLDPWD", 0))
		creat_env_var(&vars->env, old_pwd, "OLDPWD", 0);
	free(old_pwd);
	if (chdir(cmd[1]))
		err_mes(1, vars, cmd, cmd[0]);
	else
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

	if (ft_strcmp(cmd[1], "-n"))
		i = 0;
	else
		i = 1;
	while (cmd[++i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
	}
	if (ft_strcmp(cmd[1], "-n"))
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

void	free_node(t_vars *vars, char *cmd)
{
	t_env	*env;
	t_env	*tmp;
	char	*str;

	env = vars->env;
	str = ft_strjoin(cmd, "=");
	malloc_err(!str, "unset");
	while (env && env->next)
	{
		if (!ft_strncmp(env->next->line, str, ft_strlen(str)))
		{
			tmp = env->next->next;
			free(env->next->line);
			free(env->next->key);
			free(env->next->value);
			free(env->next);
			env->next = tmp;
		}
		env = env->next;
	}
	free(str);
}

void	unset(t_vars *vars, char **cmd)
{
	int		i;

	i = 0;
	vars->exit_stat = 0;
	while (cmd[++i])
	{
		if (!ft_isdigit(*cmd[i]) && ft_isalnum_str(cmd[i], 'u'))
			free_node(vars, cmd[i]);
		else
		{
			vars->exit_stat = 1;
			err_mes(1, vars, cmd, cmd[i]);
		}
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
