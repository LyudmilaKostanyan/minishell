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
	malloc_err(!pwd, vars->cmd[0]);
	printf("%s\n", pwd);
	free(pwd);
	vars->exit_stat = 0;
}

void	cd(t_vars *vars)
{
	char	*pwd;
	t_env	*tmp;

																			// oldpwd
	if (chdir(vars->cmd[1]))												// meh
		err_mes(1, vars, vars->cmd[1], "No such file or directory");
	else
	{
		pwd = getcwd(NULL, 0);
		malloc_err(!pwd, vars->cmd[0]);
		tmp = vars->env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->line, "PWD=", 4))
			{
				free(tmp->line);
				tmp->line = ft_strjoin("PWD=", pwd);
				malloc_err(!tmp->line, vars->cmd[0]);
				break ;
			}
			tmp = tmp->next;
		}
		free(pwd);
		vars->exit_stat = 0;
	}
}

void	echo(t_vars *vars)
{
	int	i;

	if (ft_strcmp(vars->cmd[1], "-n"))
		i = 0;
	else
		i = 1;
	while (vars->cmd[++i])
	{
		printf("%s", vars->cmd[i]);
		if (vars->cmd[i + 1])
			printf(" ");
	}
	if (ft_strcmp(vars->cmd[1], "-n"))
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

void	unset(t_vars *vars)
{
	t_env	*list;
	t_env	*tmp;
	int		i;
	char	*str;

	i = 0;
	vars->exit_stat = 0;
	while (vars->cmd[++i])
	{
		if (!ft_isdigit(*vars->cmd[i]) && ft_isalnum_str(vars->cmd[i], 'u'))
		{
			list = vars->env;
			str = ft_strjoin(vars->cmd[i], "=");
			malloc_err(!str, vars->cmd[0]);
			while (list && list->next)
			{
				if (!ft_strncmp(list->next->line, str, ft_strlen(str)))
				{
					tmp = list->next->next;
					free(list->next->line);
					free(list->next->key);
					free(list->next->value);
					free(list->next);
					list->next = tmp;
				}
				list = list->next;
			}
			free(str);
		}
		else
		{
			vars->exit_stat = 0;
			err_mes(1, vars, vars->cmd[i], "not a valid identifier");
		}
	}
}

void	exit_prog(t_vars *vars)
{
	long long	exit_code;

	if (vars->cmd[1])
	{
		exit_code = ft_atoll(vars->cmd[1]);
		exit(exit_code % 256);
	}
	else
		exit(0);
}
