/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 22:33:49 by lykostan          #+#    #+#             */
/*   Updated: 2023/07/31 23:56:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	check_set_help(t_env *env, char *cmd, long long equal)
{
	if (equal > 0)
	{
		if (!ft_strcmp(ft_strchr(cmd, '=') + 1, env->value))
			return ;
		free(env->value);
		env->value = ft_substr(cmd, equal + 1, ft_strlen(cmd) - equal);
	}
	else
	{
		if (!ft_strcmp(cmd, env->value))
			return ;
		free(env->value);
		env->value = ft_strdup(cmd);
	}
	malloc_err(!env->value, "check_set : env->value");
	free(env->line);
	merge_key_value(env);
}

int	check_set(t_env *env, char *cmd, char *key, long long equal)
{
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
	check_set_help(env, cmd, equal);
	return (1);
}
