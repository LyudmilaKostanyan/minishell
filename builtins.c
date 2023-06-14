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
#include <libft.h>

void	pwd(t_vars *vars)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	stop_program(!pwd, "history", MALL_ERR, vars->exit_stat);
	printf("%s\n", pwd);
	free(pwd);
	vars->exit_stat = 1;
}

void	cd(t_vars *vars)
{
	char	*pwd;
	t_env	*tmp;

	// oldpwd
	if (chdir(vars->cmd[1]))
	{
		vars->exit_stat = 1;
		perror("cd: ");
	}
	else
	{
		pwd = getcwd(NULL, 0);
		stop_program(!pwd, "history", MALL_ERR, vars->exit_stat);
		tmp = vars->env;
		while (tmp)
		{
			if (!ft_strncmp(tmp->line, "PWD=", 4))
			{
				free(tmp->line);
				tmp->line = ft_strjoin("PWD=", pwd);
				stop_program(!tmp->line, "history", MALL_ERR, vars->exit_stat);
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
	vars->exit_stat = 1;
}

void	env(t_vars *vars)
{
	t_env	*tmp;

	tmp = vars->env;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	vars->exit_stat = 1;
}

//other simbols
void	export(t_vars *vars)
{
	t_env	*tmp;
	int		i;

	i = 0;
	while (vars->cmd[++i])
	{
		if (ft_strchr(vars->cmd[i], '='))
		{
			if (!ft_isdigit(*vars->cmd[i]) && *vars->cmd[i] != '=')
			{
				tmp = vars->env;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = malloc(sizeof(t_env));
				stop_program(!tmp->next, "history", MALL_ERR, vars->exit_stat);
				tmp->next->line = ft_strdup(vars->cmd[i]);
				stop_program(!tmp->next->line, "history",
					MALL_ERR, vars->exit_stat);
				tmp->next->next = NULL;
				vars->exit_stat = 1;
			}
			else
			{
				vars->exit_stat = 0;
				printf("minishell: export: `%s': not a valid identifier",
					vars->cmd[i]);
			}
		}
	}
}

void	unset(t_vars *vars)
{
	t_env	*list;
	t_env	*tmp;
	int		i;
	char	*str;

	i = 0;
	while (vars->cmd[++i])
	{
		if (ft_isdigit(*vars->cmd[i]) || ft_strchr(vars->cmd[i], '=') || !ft_isalnum_str(vars->cmd[i]))
			printf("error\n"); //!!!!!!!!!!!!
		list = vars->env;
		str = ft_strjoin(vars->cmd[i], "=");
		//malloc err
		while (list && list->next)
		{
			if (!ft_strncmp(list->next->line, str, ft_strlen(str)))
			{
				tmp = list->next->next;
				free(list->next->line);
				free(list->next);
				list->next = tmp;
			}
			list = list->next;
		}
		free(str);
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
