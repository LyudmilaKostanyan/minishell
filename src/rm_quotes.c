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
	malloc_err(!*out_str, "rm_quotes", vars);
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

int	rm_q_if(t_vars *vars, char **tmp, int *i)
{
	int	here_doc;
	int	j;

	here_doc = 0;
	if (**tmp == '<' && *(*tmp + 1) && *(*tmp + 1) == **tmp)
	{
		here_doc++;
		j = 1;
		vars->hd_stat = 0;
		while ((*tmp)[++j] == 32)
			;
		if ((*tmp)[j] == '\'' || (*tmp)[j] == '\"')
			vars->hd_stat = 1;
	}
	if (*(*tmp + 1) && *(*tmp + 1) == **tmp
		&& *(*tmp + 2) != 32 && *(*tmp + 2) != **tmp)
	{
		vars->out_str[++(*i)] = *(*tmp + 1);
		vars->out_str[++(*i)] = 32;
		(*tmp)++;
	}
	else if (*(*tmp + 1) && *(*tmp + 1) != 32
		&& *(*tmp + 1) != **tmp)
		vars->out_str[++(*i)] = 32;
	return (here_doc);
}

void	rm_q_else(t_vars *vars, char **tmp, int *i, char main_c)
{
	t_env	*env;
	int		here_doc;

	here_doc = 0;
	if (main_c != '\'' && **tmp == '$' && (*(*tmp + 1) == '_'
			|| ft_isalpha(*(*tmp + 1))) && *(*tmp + 1) != '?' && !here_doc)
	{
		env = find_same_key(*vars, *tmp);
		if (env)
			fill_out_str(tmp, &vars->out_str, env, i);
		else
			while (*(++(*tmp)) && *(*tmp + 1) && *(*tmp + 1) != '$'
				&& **tmp != 1 && **tmp != main_c && **tmp != 32)
					;
	}
	else
	{
		vars->out_str[++(*i)] = **tmp;
		if ((**tmp == '>' || **tmp == '<'))
			here_doc += rm_q_if(vars, tmp, i);
	}
}

char	*rm_quotes(t_vars *vars, char *input_str)
{
	int		i;
	char	main_c;

	find_main_c(vars, input_str);
	if (!creat_out_str(vars, input_str, &vars->out_str))
		return (NULL);
	i = -1;
	while (*input_str)
	{
		if (*input_str == vars->main_c)
		{
			if (--vars->q_count % 2 == 0)
			{
				find_main_c(vars, input_str + 1);
				main_c = 0;
			}
			else
				main_c = vars->main_c;
		}
		else
			rm_q_else(vars, &input_str, &i, main_c);
		input_str++;
	}
	vars->out_str[++i] = 0;
	return (vars->out_str);
}
