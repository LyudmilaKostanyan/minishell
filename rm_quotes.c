/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:08:29 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/28 14:08:31 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*key_cmp(t_vars vars, char *input_str, long long keys_end)
{
	char	*key;

	key = ft_substr(input_str, 0, keys_end);
	malloc_err(!key, "check $ case");
	while (vars.env)
	{
		if (!ft_strcmp(vars.env->key, key))
		{
			free(key);
			return (vars.env);
		}
		vars.env = vars.env->next;
	}
	while (vars.env_vars)
	{
		if (!ft_strcmp(vars.env_vars->key, key))
		{
			free(key);
			return (vars.env_vars);
		}
		vars.env_vars = vars.env_vars->next;
	}
	free(key);
	return (NULL);
}

t_env	*find_same_key(t_vars vars, char *input_str)
{
	long long	keys_end;
	t_env		*env;

	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
		{
			input_str++;
			if (ft_strchr(input_str, 1))
				keys_end = ft_strchr(input_str, 1) - input_str;
			else if (ft_strchr(input_str, 32))
				keys_end = ft_strchr(input_str, 32) - input_str;
			else if (ft_strchr(input_str, vars.main_c))
				keys_end = ft_strchr(input_str, vars.main_c) - input_str;
			else
				keys_end = ft_strlen(input_str);
			env = key_cmp(vars, input_str, keys_end);
			if (env)
				return (env);
		}
		input_str++;
	}
	return (NULL);
}

t_env	*creat_out_str(t_vars *vars, char *input_str, char **out_str)
{
	int		val_len;
	int		key_len;
	t_env	*env;

	val_len = 0;
	key_len = 0;
	env = find_same_key(*vars, input_str);
	if (env)
	{
		key_len = ft_strlen(env->key) + 1;
		val_len = ft_strlen(env->value);
	}
	else if (!vars->main_c)
		return (NULL);
	if (vars->main_c != '\'')
		*out_str = malloc(ft_strlen(input_str) + val_len
				- key_len - vars->q_count + 1);
	else
		*out_str = malloc(ft_strlen(input_str) - vars->q_count + 1);
	malloc_err(!*out_str, "rm_quotes");
	return (env);
}

void	fill_out_str(char **tmp, char **out_str, t_env *env, int *i)
{
	int	j;

	j = -1;
	while (env->value[++j])
		(*out_str)[++(*i)] = env->value[j];
	(*tmp)++;
	j = -1;
	while (*(*tmp + 1) && **tmp == env->key[++j])
		(*tmp)++;
}

char	*rm_quotes(t_vars *vars, char *input_str)
{
	char	*out_str;
	int		i;
	char	*tmp;
	t_env	*env;

	env = creat_out_str(vars, input_str, &out_str);
	if (!env)
		return (NULL);
	i = -1;
	tmp = input_str;
	while (*tmp)
	{
		if (*tmp != vars->main_c)
		{
			if (vars->main_c != '\'' && *tmp == '$' && env)
				fill_out_str(&tmp, &out_str, env, &i);
			else
				out_str[++i] = *tmp;
		}
		tmp++;
	}
	out_str[++i] = 0;
	vars->main_c = 0;
	return (out_str);
}
