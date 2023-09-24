/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:23:25 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 19:23:26 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipes(char *input_str)
{
	int	i;
	int	len;

	len = ft_strlen(input_str);
	i = -1;
	while (input_str[++i] == 32)
		;
	if (input_str[i] && input_str[i] == '|')
		return (0);
	while (++i < len)
	{
		if (input_str[i] == '|')
		{
			while (input_str[++i] == 32)
				;
			if (input_str[i] == '|' || !input_str[i])
				return (0);
		}
	}
	return (1);
}

int	check_input(t_vars *vars, char *input_str, char ***p_splt, char **for_splt)
{
	stop_program(!input_str, NULL, "exit", vars);
	if (!*input_str)
	{
		free(input_str);
		return (-1);
	}
	if (!check_pipes(input_str))
	{
		add_history(input_str);
		free(input_str);
		return (0);
	}
	if (!quotes_handler(vars, &input_str))
	{
		add_history(input_str);
		free(input_str);
		return (-1);
	}
	split_free(*p_splt);
	*for_splt = rm_quotes(vars, input_str);
	if (!*for_splt)
		*p_splt = ft_split(input_str, '|');
	else
		*p_splt = ft_split(*for_splt, '|');
	return (1);
}

int	read_input(t_vars *vars, t_cmds **cmds)
{
	char	*input_str;
	char	**pipe_splt;
	char	*for_splt;
	int		count;
	int		cond;

	pipe_splt = NULL;
	*cmds = NULL;
	input_str = readline("\e[34mminishell$ \e[0m");
	cond = check_input(vars, input_str, &pipe_splt, &for_splt);
	if (!cond || cond == -1)
		return (cond);
	free(for_splt);
	malloc_err(!pipe_splt, "split cmds", vars);
	restore_spaces(&input_str);
	count = split_size(pipe_splt);
	free(*cmds);
	*cmds = malloc((sizeof(t_cmds) * count) + 1);
	malloc_err(!*cmds, "creat cmds", vars);
	count = merge_cmds(vars, cmds, pipe_splt, count);
	add_history(input_str);
	free(input_str);
	return (count);
}
