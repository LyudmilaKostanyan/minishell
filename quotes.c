/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 14:08:19 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/28 14:08:23 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_input(char **input_str)
{
	char	*line;
	char	*tmp;

	tmp = ft_strjoin(*input_str, "\n");
	malloc_err(!tmp, "reading input");
	free(*input_str);
	*input_str = tmp;
	line = readline("> ");
	if (!line)
	{
		printf("%s%s", QUOTES_ERR, QUOTES_SYN_ERR); // ^D ??????????
		return (NULL);
	}
	tmp = ft_strjoin(*input_str, line);
	malloc_err(!tmp, "reading input");
	free(*input_str);
	*input_str = tmp;
	return (line);
}

void	wait_quote(char **input_str, char c, int *count)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		line = join_input(input_str);
		if (!line)
			break ;
		tmp = ft_strchr(line, c);
		if (tmp)
		{
			(*count)++;
			if (!ft_strchr(tmp + 1, c))
			{
				break ;
				free(line);
			}
		}
		free(line);
	}
}

void	find_main_c(t_vars *vars, char *tmp)
{
	while (*tmp)
	{
		if (*tmp == '\'' || *tmp == '\"')
		{
			vars->main_c = *tmp;
			break ;
		}
		tmp++;
	}
}

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

void	quotes_handler(t_vars *vars, char **input_str)
{
	int		i;
	int		count;

	vars->q_count = quotes_counter(vars, *input_str);
	if (!vars->q_count)
		return ;
	if (vars->q_count % 2 != 0)
		wait_quote(input_str, vars->main_c, &vars->q_count);
	count = vars->q_count;
	i = -1;
	while ((*input_str)[++i])
	{
		if ((*input_str)[i] == vars->main_c)
		{
			while ((*input_str)[i] && (*input_str)[i + 1] && count)
			{
				if ((*input_str)[i] == vars->main_c)
					count--;
				if ((*input_str)[i] == 32)
					(*input_str)[i] = 1;
				i++;
			}
		}
		if ((*input_str)[i] && !count)
		{
			count = quotes_counter(vars, *input_str + i);
			if (!count)
				continue ;
			if (count % 2 != 0)
				wait_quote(input_str, vars->main_c, &count);
			vars->q_count += count;
		}
	}
}
