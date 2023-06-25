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

void	merge_key_value(t_env *node, char *err)
{
	char	*key;

	key = ft_strjoin(node->key, "=");
	malloc_err(!key, err);
	node->line = ft_strjoin(key, node->value);
	malloc_err(!node->line, err);
	free(key);
}

t_env	*checking_env_key(t_env *env, char *key)
{
	if (key[ft_strlen(key) - 1] == '+')
		key[ft_strlen(key) - 1] = 0;
	while (env)
	{
		if (!ft_strcmp(key, env->key))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	add_value(char *cmd, t_env **env)
{
	char	*tmp;

	tmp = ft_strjoin((*env)->value, ft_strchr(cmd, '=') + 1);
	malloc_err(!tmp, "add value in env variable");
	free((*env)->value);
	(*env)->value = tmp;
	tmp = ft_strjoin((*env)->line, ft_strchr(cmd, '=') + 1);
	malloc_err(!tmp, "add value in env variable");
	free((*env)->line);
	(*env)->line = tmp;
}

int	check_env_vars(t_env *env, char *cmd, char *key, long long equal)
{
	// t_env	*env;
	int		plus;

	plus = 0;
	if (key[ft_strlen(key) - 1] == '+')
		plus++;
	env = checking_env_key(env, key);
	if (!env)
		return (0);
	if (plus)
	{
		add_value(cmd, &env);
		return (1);
	}
	if (equal > 0)
	{
		if (!ft_strcmp(ft_strchr(cmd, '=') + 1, env->value))
			return (1);
		free(env->value);
		env->value = ft_substr(cmd, equal + 1, ft_strlen(cmd) - equal);
	}
	else
	{
		if (!ft_strcmp(cmd, env->value))
			return (1);
		free(env->value);
		env->value = ft_strdup(cmd);
	}
	malloc_err(!env->value, "sadfg");					//???????????????
	free(env->line);
	merge_key_value(env, "asdfg");
	return (1);
}

void	creat_env_var(t_env **env, char *cmd, char *key, long long equal)
{
	t_env	*head;
	t_env	*tmp;

	head = *env;
	while (*env)
	{
		if (!(*env)->next)
			tmp = *env;
		*env = (*env)->next;
	}
	(*env) = malloc(sizeof(t_env));
	malloc_err(!*env, "asdfgh");		//????????
	if (head)
		tmp->next = *env;
	if (key[ft_strlen(key) - 1] == '+')
		(*env)->key = ft_substr(key, 0, ft_strlen(key) - 1);
	else
		(*env)->key = ft_strdup(key);
	if (equal > 0)
		(*env)->value = ft_substr(cmd, equal + 1,
				ft_strlen(cmd) - equal);
	else
		(*env)->value = ft_strdup(cmd);
	malloc_err(!(*env)->key || !(*env)->value, "asdfgh");	//????????
	merge_key_value(*env, "dsasdas");		//?????
	(*env)->next = NULL;
	if (head)
		*env = head;
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
		equal = (uintptr_t)ft_strchr(vars->cmd[i], '=')
			- (uintptr_t)vars->cmd[i];
		key = ft_substr(vars->cmd[i], 0, equal);
		malloc_err(!key, vars->cmd[0]);
		if (!ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e') && equal >= 0
			&& !check_env_vars(vars->env, vars->cmd[i], key, equal))
			creat_env_var(&vars->env, vars->cmd[i], key, equal);
		else if (ft_isdigit(*key) || !*key || !ft_isalnum_str(key, 'u'))
		{
			err_mes(1, vars, vars->cmd[i], "not a valid identifier");
			vars->exit_stat = 1;
		}
		free(key);
	}
}
