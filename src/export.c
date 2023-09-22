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

int	check_set_elfi(char *cmd, char **env_value, char **new_value)
{
	if (!ft_strcmp(cmd, *env_value))
		return (1);
	free(*env_value);
	*env_value = *new_value;
	return (0);
}

int	check_set(t_vars *vars, t_env *env, char *cmd, char *key)
{
	int		plus;
	char	*tmp;

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
		tmp = ft_substr(cmd, vars->equal + 1, ft_strlen(cmd) - vars->equal);
		if (check_set_elfi(ft_strchr(cmd, '=') + 1, &env->value, &tmp))
			return (1);
	}
	else
	{
		tmp = ft_strdup(cmd);
		if (check_set_elfi(cmd, &env->value, &tmp))
			return (1);
	}
	malloc_err(!env->value, "check_set : env->value", vars->true_env);
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
	malloc_err(!*env, "creat_env_var: env", vars->true_env);
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
		vars->true_env);
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
		malloc_err(!vars->key, cmd[0], vars->true_env);
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
