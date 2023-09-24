/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/17 16:35:51 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 20:03:58 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int
	check_set_elfi(char *cmd, char **env_value, char *new_value, t_vars *vars)
{
	malloc_err(!new_value, "new_value", vars);
	if (!ft_strcmp(cmd, *env_value))
	{
		free(new_value);
		return (1);
	}
	free(*env_value);
	*env_value = new_value;
	return (0);
}

int	check_set(t_vars *vars, t_env *env, char *cmd, char *key)
{
	int		plus;

	plus = 0;
	if (key[ft_strlen(key) - 1] == '+')
		plus++;
	env = checking_env_key(env, key);
	if (!env)
		return (0);
	if (plus)
		return (add_value(vars, cmd, &env));
	if (vars->equal > 0)
	{
		if (check_set_elfi(ft_strchr(cmd, '=') + 1, &env->value,
				ft_substr(cmd, vars->equal + 1,
					ft_strlen(cmd) - vars->equal), vars))
			return (1);
	}
	else
		if (check_set_elfi(cmd, &env->value, ft_strdup(cmd), vars))
			return (1);
	malloc_err(!env->value, "check_set : env->value", vars);
	free(env->line);
	merge_key_value(vars, env);
	return (1);
}

t_env	*init_node(t_vars *vars, t_env **env)
{
	t_env	*tmp;

	while (*env)
	{
		if (!(*env)->next)
			tmp = *env;
		*env = (*env)->next;
	}
	(*env) = malloc(sizeof(t_env));
	malloc_err(!*env, "creat_env_var: env", vars);
	return (tmp);
}

void	creat_env_var(t_vars *vars, t_env **env, char *cmd, char *key)
{
	t_env	*head;
	t_env	*tmp;

	head = *env;
	tmp = init_node(vars, env);
	if (head)
		tmp->next = *env;
	if (key[ft_strlen(key) - 1] == '+')
		(*env)->key = ft_substr(key, 0, ft_strlen(key) - 1);
	else
		(*env)->key = ft_strdup(key);
	if (vars->equal > 0)
		(*env)->value = ft_substr(cmd, vars->equal + 1,
				ft_strlen(cmd) - vars->equal);
	else
		(*env)->value = ft_strdup(cmd);
	malloc_err(!(*env)->key || !(*env)->value, "creat_env_var: key/value",
		vars);
	merge_key_value(vars, *env);
	(*env)->next = NULL;
	if (head)
		*env = head;
}

void	export(t_vars *vars, char **cmd)
{
	int			i;

	i = 0;
	g_exit_status = 0;
	if (!cmd[1])
		env(vars, 0);
	while (cmd[++i])
	{
		vars->equal = ft_strchr(cmd[i], '=') - cmd[i];
		vars->key = ft_substr(cmd[i], 0, vars->equal);
		malloc_err(!vars->key, cmd[0], vars);
		if (!ft_isdigit(*vars->key) && *vars->key
			&& ft_isalnum_str(vars->key, 'e') && vars->equal >= 0
			&& !check_set(vars, vars->env, cmd[i], vars->key))
			creat_env_var(vars, &vars->env, cmd[i], vars->key);
		else if (ft_isdigit(*vars->key) || !*vars->key
			|| !ft_isalnum_str(vars->key, 'u'))
		{
			err_mes(1, *cmd, cmd[i], E_U_ERR);
			g_exit_status = 1;
		}
		free(vars->key);
	}
}
