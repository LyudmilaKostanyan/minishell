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

void	quotes_counter_help(char input_str, int *count, char *main_c, int *stat)
{
	if (input_str == '\'' || input_str == '\"')
	{
		if (!*stat)
		{
			(*stat)++;
			(*count)++;
			*main_c = input_str;
		}
		else if (*main_c == input_str)
		{
			*stat = 0;
			(*count)++;
			*main_c = 0;
		}
	}
}

int	quotes_counter(char **input_str)
{
	int		i;
	char	main_c;
	int		count;
	int		stat;

	count = 0;
	stat = 0;
	main_c = 0;
	i = -1;
	while ((*input_str)[++i])
	{
		quotes_counter_help((*input_str)[i], &count, &main_c, &stat);
		if (main_c && (*input_str)[i] == 32)
			(*input_str)[i] = 1;
	}
	return (count);
}

int	quotes_handler(t_vars *vars, char **input_str)
{
	vars->q_count = quotes_counter(input_str);
	if (!vars->q_count)
		return (1);
	if (err_mes(vars->q_count % 2 != 0, NULL, QUOTES_ERR, vars))
		return (0);
	return (1);
}
