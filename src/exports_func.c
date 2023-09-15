/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exports_func.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:49:42 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/15 18:49:43 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_key_value(t_vars *vars, t_env *node)
{
	char	*key;

	key = ft_strjoin(node->key, "=");
	malloc_err(!key, "merge_key_value", vars->true_env);
	node->line = ft_strjoin(key, node->value);
	malloc_err(!node->line, "merge_key_value", vars->true_env);
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

int	add_value(t_vars *vars, char *cmd, t_env **env)
{
	char	*tmp;

	tmp = ft_strjoin((*env)->value, ft_strchr(cmd, '=') + 1);
	malloc_err(!tmp, "add value in env variable", vars->true_env);
	free((*env)->value);
	(*env)->value = tmp;
	tmp = ft_strjoin((*env)->line, ft_strchr(cmd, '=') + 1);
	malloc_err(!tmp, "add value in env variable", vars->true_env);
	free((*env)->line);
	(*env)->line = tmp;
	return (1);
}
