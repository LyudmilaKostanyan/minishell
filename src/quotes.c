/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:08:19 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 19:40:13 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	quotes_counter(t_vars *vars, char *input_str)
{
	int		count;

	find_main_c(vars, input_str);
	count = 0;
	while (*input_str)
	{
		if (*input_str == vars->main_c)
			count++;
		input_str++;
	}
	return (count);
}

int	rm_spaces(t_vars *vars, char **input_str, int *count, int *i)
{
	if ((*input_str)[*i] == vars->main_c)
	{
		while ((*input_str)[*i] && (*input_str)[*i + 1] && count)
		{
			if ((*input_str)[*i] == vars->main_c)
				(*count)--;
			if ((*input_str)[*i] == 32 && *count % 2 != 0)
				(*input_str)[*i] = 1;
			(*i)++;
		}
	}
	if ((*input_str)[*i] && !*count)
	{
		*count = quotes_counter(vars, *input_str + *i);
		if (!*count)
			return (1);
		vars->q_count += *count;
	}
	return (0);
}

int	quotes_handler(t_vars *vars, char **input_str)
{
	int		i;
	int		count;

	vars->q_count = quotes_counter(vars, *input_str);
	if (!vars->q_count)
		return (1);
	if (err_mes(vars->q_count % 2 != 0, NULL, NULL, QUOTES_ERR))
		return (0);
	count = vars->q_count;
	i = -1;
	while ((*input_str)[++i])
		if (rm_spaces(vars, input_str, &count, &i))
			continue ;
	return (1);
}
