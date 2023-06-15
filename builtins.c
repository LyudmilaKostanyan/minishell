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

void	env(t_vars *vars)
{
	t_env	*tmp;

	tmp = vars->env;
	while (tmp)
	{
		printf("%s\n", tmp->line);
		tmp = tmp->next;
	}
	vars->exit_stat = 0;
}

int	chech(char *cmd) //nameing suka
{
	t_env	*tmp;
	int		stat;

	stat = 0;
	while (tmp)
	{
		if (!ft_strcmp(cmd, tmp))
			stat = 1;
		tmp = tmp->next;
	}
	return (stat);

}

int	check_export_env(t_vars *vars, int i, char *key)
{
	t_env	*tmp;
	int		stat;

	tmp = vars->env;
	if (key[ft_strlen(key) - 1] != '+')
		if (!check(vars->cmd[i], i))
			return (0);
	else
	{
		key[ft_strlen(key) - 1] = '='
		if ()
	}
	//+ case
	//other value case
}

//same varisble in env
void	export(t_vars *vars)
{
	t_env	*tmp;
	int		i;
	int		equal;
	char	*key;

	i = 0;
	vars->exit_stat = 0;
	while (vars->cmd[++i])
	{
		equal = ft_strchr(vars->cmd[i], '=') - vars->cmd[i];
		if (equal >= 0)
		{
			key = ft_substr(vars->cmd[i], 0, equal);
			malloc_err(!key, vars->cmd[0]);
			if (!ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e'))
			{
				tmp = vars->env;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = malloc(sizeof(t_env));
				malloc_err(!tmp->next, vars->cmd[0]);
				if (key[ft_strlen(key) - 1] == '+')
				{
					tmp->next->key = ft_substr(key, 0, ft_strlen(key) - 1);
					free(key);
				}
				else
					tmp->next->key = key;
				tmp->next->value = ft_substr(vars->cmd[i], equal + 1,
					ft_strlen(vars->cmd[i]) - equal);
				malloc_err(!tmp->next->key || !tmp->next->value, vars->cmd[0]);
				key = ft_strjoin(tmp->next->key, "=");
				malloc_err(!key, vars->cmd[0]);
				tmp->next->line = ft_strjoin(key, tmp->next->value);
				malloc_err(!tmp->next->line, vars->cmd[0]);
				free(key);
				tmp->next->next = NULL;
			}
			else
			{
				err_mes(1, vars, vars->cmd[i], "not a valid identifier");
				vars->exit_stat = 1;
				free(key);
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
			printf("asdfg\n");
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
