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

t_env	*key_cmp(t_vars vars, char *key)
{
	// printf("key: %s\n", key);
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

t_env	*meh(t_vars vars, char **input_str)		//nameing
{
	int		keys_end;
	t_env	*env;
	char	*tmp;
	char	*key;

	keys_end = 0;
	tmp = (*input_str)++;
	while (*(++tmp) && *tmp != '/' && *tmp != '$' && *tmp != 1
		&& *tmp != '\"' && *tmp != '\'' && *tmp != 32)
			keys_end++;
	if (*(tmp - 1) == '/')
		keys_end++;
	key = ft_substr(*input_str, 0, keys_end);
	malloc_err(!key, "check $ case");
	env = key_cmp(vars, key);
	free(key);
	return (env);
}

t_env	*find_same_key(t_vars vars, char *input_str)
{
	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
			return(meh(vars, &input_str));
		input_str++;
	}
	return (NULL);
}

int	count_key_val(t_vars vars, char *input_str, int *key_len, int *val_len)
{
	t_env		*env;
	int		q_count;
	int		cond;

	cond = 0;
	q_count = vars.q_count;
	find_main_c(&vars, input_str);
	while (*input_str)
	{
		if (*input_str == vars.main_c)
			if (--q_count % 2 == 0)
				find_main_c(&vars, input_str + 1);
		// printf("%c\n", vars.main_c);
		if (*input_str == '$' && *(input_str + 1) && vars.main_c != '\'')
		{
			cond++;
			env = meh(vars, &input_str);
			if (env)
			{
				*key_len += ft_strlen(env->key) + 1;
				*val_len += ft_strlen(env->value);
			}
		}
		input_str++;
	}
	return (cond);
}

int	creat_out_str(t_vars *vars, char *input_str, char **out_str)
{
	int		val_len;
	int		key_len;
	int		cond;

	val_len = 0;
	key_len = 0;
	cond = count_key_val(*vars, input_str, &key_len, &val_len);
	if (!vars->main_c && !key_len && !cond)
		return (0);
	// printf("key_len: %d\tval_len: %d\tmall_len: %zu\n", key_len, val_len, ft_strlen(input_str) + val_len - key_len - vars->q_count + 1);
	*out_str = malloc(ft_strlen(input_str) + val_len
			- key_len - vars->q_count + 1);
	malloc_err(!*out_str, "rm_quotes");
	return (1);
}

void	fill_out_str(char **tmp, char **out_str, t_env *env, int *i)
{
	int	j;

	j = -1;
	while (env->value[++j])
		(*out_str)[++(*i)] = env->value[j];
	(*tmp)++;
	j = -1;
	while (*(*tmp + 1) && *(*tmp + 1) == env->key[++j])
		(*tmp)++;
	if (**tmp == '/')
		(*out_str)[++(*i)] = **tmp;
}

char	*rm_quotes(t_vars *vars, char *input_str)
{
	char	*out_str;
	int		i;
	char	*tmp;
	t_env	*env;

	if (!creat_out_str(vars, input_str, &out_str))
		return (NULL);
	i = -1;
	tmp = input_str;
	find_main_c(vars, tmp);
	while (*tmp)
	{
		if (*tmp != vars->main_c)
		{
			if (vars->main_c != '\'' && *tmp == '$')
			{
				env = find_same_key(*vars, tmp);
				if (env)
					fill_out_str(&tmp, &out_str, env, &i);
				else
					while (*(++tmp) && *(tmp + 1) && *(tmp + 1) != '$' && *tmp != 1
						&& *tmp != vars->main_c && *tmp != 32)
							;
			}
			else
				out_str[++i] = *tmp;
		}
		else
			if (--vars->q_count % 2 == 0)
				find_main_c(vars, tmp + 1);
		tmp++;
	}
	out_str[++i] = 0;
	vars->main_c = 0;
	return (out_str);
}
