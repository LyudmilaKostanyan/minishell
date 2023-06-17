/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 16:35:51 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/17 16:35:54 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_key_value(t_vars *vars, t_env *node)
{
	char	*key;

	key = ft_strjoin(node->key, "=");
	malloc_err(!key, vars->cmd[0]);
	node->line = ft_strjoin(key, node->value);
	malloc_err(!node->line, vars->cmd[0]);
	free(key);
}

int	check_export_env(t_vars *vars, char *cmd, char *key, long long equal)
{
	t_env	*env;
	int		stat;

	stat = 1;
	if (key[ft_strlen(key) - 1] == '+')
		key[ft_strlen(key) - 1] = 0;
	env = vars->env;
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			break ;
		env = env->next;
	}
	if (!env)
		return (0);
	if (!ft_strcmp(ft_strchr(cmd, '=') + 1, env->value))
		return (1);
	free(env->value);
	env->value = ft_substr(cmd, equal + 1, ft_strlen(cmd) - equal);
	malloc_err(!env->value, vars->cmd[0]);
	free(env->line);
	merge_key_value(vars, env);
	return (1);
}

void	creat_env_var(t_vars *vars, char *key, int i, long long equal)
{
	t_env	*tmp;

	tmp = vars->env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = malloc(sizeof(t_env));
	malloc_err(!tmp->next, vars->cmd[0]);
	if (key[ft_strlen(key) - 1] == '+')
		tmp->next->key = ft_substr(key, 0, ft_strlen(key) - 1);
	else
		tmp->next->key = ft_strdup(key);
	tmp->next->value = ft_substr(vars->cmd[i], equal + 1,
			ft_strlen(vars->cmd[i]) - equal);
	malloc_err(!tmp->next->key || !tmp->next->value, vars->cmd[0]);
	merge_key_value(vars, tmp->next);
	tmp->next->next = NULL;
}

void	export(t_vars *vars)
{
	int			i;
	long long	equal;
	char		*key;

	i = 0;
	vars->exit_stat = 0;
	if (!vars->cmd[1])
		env(vars, 0);
	while (vars->cmd[++i])
	{
		equal = (uintptr_t)ft_strchr(vars->cmd[i], '=') - (uintptr_t)vars->cmd[i];
		key = ft_substr(vars->cmd[i], 0, equal);
		malloc_err(!key, vars->cmd[0]);
		if (!ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e') && equal >= 0
			&& !check_export_env(vars, vars->cmd[i], key, equal))
			creat_env_var(vars, key, i, equal);
		else if (ft_isdigit(*key) || !*key || !ft_isalnum_str(key, 'u'))
		{
			err_mes(1, vars, vars->cmd[i], "not a valid identifier");
			vars->exit_stat = 1;
		}
		free(key);
	}
}
