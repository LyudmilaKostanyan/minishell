/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:50:01 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/15 18:50:03 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

t_env	*find_key(t_vars vars, char *key)
{
	while (vars.env)
	{
		if (!ft_strcmp(vars.env->key, key))
			return (vars.env);
		vars.env = vars.env->next;
	}
	while (vars.set)
	{
		if (!ft_strcmp(vars.set->key, key))
			return (vars.set);
		vars.set = vars.set->next;
	}
	return (NULL);
}

t_env	*key_cmp(t_vars vars, char **input_str)
{
	int		keys_end;
	t_env	*env;
	char	*tmp;
	char	*key;

	keys_end = 0;
	tmp = (*input_str)++;
	while (*(++tmp) && (ft_isalnum(*tmp) || *tmp == '_'))
		keys_end++;
	if (*(tmp - 1) == '/')
		keys_end++;
	key = ft_substr(*input_str, 0, keys_end);
	malloc_err(!key, "check $ case", &vars);
	env = find_key(vars, key);
	free(key);
	return (env);
}

t_env	*find_same_key(t_vars vars, char *input_str)
{
	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
			return (key_cmp(vars, &input_str));
		input_str++;
	}
	return (NULL);
}
