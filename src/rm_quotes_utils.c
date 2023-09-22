/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rm_quotes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 19:42:41 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/22 19:42:44 by tgalyaut         ###   ########.fr       */
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

void	restore_spaces(char **str)
{
	int	i;

	i = -1;
	while (*str && (*str)[++i])
		if ((*str)[i] == 1)
			(*str)[i] = 32;
}

int	key_value_len(t_vars vars, char *input_str, t_mall_size *mall_size)
{
	t_env	*env;

	env = key_cmp(vars, &input_str);
	if (env)
	{
		mall_size->key_len += ft_strlen(env->key) + 1;
		mall_size->val_len += ft_strlen(env->value);
	}
	return (1);
}

void	space_count(char *input_str, t_mall_size *mall_size, int i)
{
	if (i && (*input_str == '>' || *input_str == '<'))
	{
		if (*(input_str + 1) && *(input_str + 1) == *input_str
			&& *(input_str + 2) && *(input_str + 2) != 32)
			mall_size->sp_count++;
		else if (*(input_str + 1) && *(input_str + 1) != 32)
			mall_size->sp_count++;
	}
}

int	count_key_val(t_vars vars, char *input_str, t_mall_size *mall_size, int i)
{
	int		q_count;
	int		main_c;
	int		cond;

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
		if (i && *input_str == vars.main_c && --q_count % 2 == 0)
			find_main_c(&vars, input_str + 1);
		if (*input_str == '$' && *(input_str + 1) && main_c != '\'')
			cond = key_value_len(vars, input_str, mall_size);
		space_count(input_str, mall_size, i);
		input_str++;
	}
	return (cond);
}
