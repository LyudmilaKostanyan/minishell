/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_quotes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:08:29 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 19:15:03 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	find_main_c(t_vars *vars, char *tmp)
{
	while (*tmp)
	{
		if (*tmp == '\'' || *tmp == '\"')
			break ;
		tmp++;
	}
	vars->main_c = *tmp;
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
	malloc_err(!key, "check $ case", vars.true_env);///
	env = find_key(vars, key);
	free(key);
	return (env);
}

t_env	*find_same_key(t_vars vars, char *input_str)	//xi while
{
	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
			return(key_cmp(vars, &input_str));
		input_str++;
	}
	return (NULL);
}

int	count_key_val(t_vars vars, char *input_str, t_mall_size *mall_size, int i)
{
	t_env	*env;
	int		q_count;
	int		cond;
	int		main_c;

	cond = 0;
	main_c = 0;
	if (i)
	{
		q_count = vars.q_count;
		find_main_c(&vars, input_str);
	}
	while (*input_str)
	{
		if (*input_str == vars.main_c && q_count % 2 == 0)
			main_c = vars.main_c;
		else if (*input_str == vars.main_c && q_count % 2 != 0)
			main_c = 0;
		if (i && *input_str == vars.main_c && --q_count % 2 == 0)			//i - ????
				find_main_c(&vars, input_str + 1);
		if (*input_str == '$' && *(input_str + 1) && main_c != '\'')
		{
			cond++;
			env = key_cmp(vars, &input_str);
			if (env)
			{
				mall_size->key_len += ft_strlen(env->key) + 1;
				mall_size->val_len += ft_strlen(env->value);
			}
		}
		if (i && (*input_str == '>' || *input_str == '<'))
		{
			if (*(input_str + 1) && *(input_str + 1) == *input_str
				&&  *(input_str + 2) &&  *(input_str + 2) != 32)
					mall_size->sp_count++;
			else if (*(input_str + 1) && *(input_str + 1) != 32)
					mall_size->sp_count++;
		}
		input_str++;
	}
	return (cond);
}

int	creat_out_str(t_vars *vars, char *input_str, char **out_str)
{
	t_mall_size	mall_size;
	int			cond;

	mall_size.sp_count = 0;
	mall_size.key_len = 0;
	mall_size.val_len = 0;
	cond = count_key_val(*vars, input_str, &mall_size, 1);
	if (!vars->main_c && !mall_size.key_len && !cond && !mall_size.sp_count)
		return (0);
	*out_str = malloc(ft_strlen(input_str) + mall_size.val_len
			- mall_size.key_len - vars->q_count + mall_size.sp_count + 1);
	malloc_err(!*out_str, "rm_quotes", vars->true_env);///
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
	while (*(*tmp + 1) && *(*tmp + 1) == env->key[++j + 1])
		(*tmp)++;
}

char	*rm_quotes(t_vars *vars, char *input_str)
{
	char	*out_str;
	int		i;
	char	*tmp;
	t_env	*env;
	int		here_doc;
	char	main_c;

	tmp = input_str;
	find_main_c(vars, tmp);
	if (!creat_out_str(vars, input_str, &out_str))
		return (NULL);
	i = -1;
	here_doc = 0;
	main_c = 0;
	while (*tmp)
	{
		if (*tmp == vars->main_c)
		{
			if (--vars->q_count % 2 == 0)
			{
				find_main_c(vars, tmp + 1);
				main_c = 0;
			}
			else
				main_c = vars->main_c;
		}
		else
		{
			if (main_c != '\'' && *tmp == '$' && (*(tmp + 1) == '_'
				|| ft_isalpha(*(tmp + 1))) && *(tmp + 1) != '?' && !here_doc)
			{
				env = find_same_key(*vars, tmp);
				if (env)
					fill_out_str(&tmp, &out_str, env, &i);
				else
					while (*(++tmp) && *(tmp + 1) && *(tmp + 1) != '$' && *tmp != 1
						&& *tmp != main_c && *tmp != 32)
							;
			}
			else
			{
				out_str[++i] = *tmp;
				if ((*tmp == '>' || *tmp == '<'))
				{
					if (*tmp == '<' && *(tmp + 1) && *(tmp + 1) == *tmp)
					{
						here_doc++;
						int	j;				//norm
						j = 1;
						vars->hd_stat = 0;
						while (tmp[++j] == 32)
							;
						if (tmp[j] == '\'' || tmp[j] == '\"')
							vars->hd_stat = 1;
					}
					if(*(tmp + 1) && *(tmp + 1) == *tmp
						&& *(tmp + 2) != 32 && *(tmp + 2) != *tmp)
					{
						out_str[++i] = *(tmp + 1);
						out_str[++i] = 32;
						tmp++;
					}
					else if (*(tmp + 1) && *(tmp + 1) != 32 && *(tmp + 1) != *tmp)
						out_str[++i] = 32;
				}
			}
		}
		tmp++;
	}
	out_str[++i] = 0;
	vars->main_c = 0;
	return (out_str);
}
