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

void	rm_q_if(t_vars *vars, char **tmp, int *i, char main_c)
{
	int		j;

	if (**tmp == '<' && *(*tmp + 1) && *(*tmp + 1) == **tmp)
	{
		vars->here_doc++;
		j = 2;
		vars->hd_stat = 0;
		while ((*tmp)[j] && (*tmp)[j] != 32 && (*tmp)[j] != main_c)
			j++;
		vars->hd_end = *tmp + j + 1;
		if ((*tmp)[j - 1] == '\'' || (*tmp)[j - 1] == '\"')
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
}

void	replace_exit_stat(t_vars *vars, char **tmp, int *i)
{
	size_t	j;
	char	*exit_stat_num;

	j = -1;
	exit_stat_num = ft_itoa(g_exit_status);
	while (++j < ft_strlen(exit_stat_num))
		vars->out_str[++(*i)] = exit_stat_num[j];
	free(exit_stat_num);
	(*tmp) += 1;
}

void	rm_q_elseelse(t_vars *vars, char **tmp, int *i, char main_c)
{
	if (**tmp == '<' && main_c)
		vars->out_str[++(*i)] = 2;
	else if (**tmp == '>' && main_c)
		vars->out_str[++(*i)] = 3;
	else if (**tmp == '|' && main_c)
		vars->out_str[++(*i)] = 4;
	else
		vars->out_str[++(*i)] = **tmp;
	if (*(*tmp + 1) && **tmp != 32 && **tmp != '>' && **tmp != '<'
		&& (*(*tmp + 1) == '>' || *(*tmp + 1) == '<'))
	{
		vars->out_str[++(*i)] = 32;
		vars->out_str[++(*i)] = *(*tmp + 1);
		(*tmp)++;
	}
	if ((**tmp == '>' || **tmp == '<') && !main_c)
		rm_q_if(vars, tmp, i, main_c);
}

void	rm_q_else(t_vars *vars, char **tmp, int *i, char main_c)
{
	t_env	*env;

	if (*tmp == vars->hd_end)
		vars->here_doc = 0;
	if (main_c != '\'' && **tmp == '$' && *(*tmp + 1)
		&& (*(*tmp + 1) == '_' || ft_isalpha(*(*tmp + 1))
			|| *(*tmp + 1) == '?') && !vars->here_doc)
	{
		if (*(*tmp + 1) == '?')
		{
			replace_exit_stat(vars, tmp, i);
			return ;
		}
		env = find_same_key(*vars, *tmp);
		if (env)
			fill_out_str(tmp, &vars->out_str, env, i);
		else
			while (*(++(*tmp)) && *(*tmp + 1) && *(*tmp + 1) != '$'
				&& **tmp != 1 && **tmp != main_c && **tmp != 32)
					;
	}
	else
		rm_q_elseelse(vars, tmp, i, main_c);
}

void	rm_quotes(t_vars *vars, char *input_str)
{
	int		i;
	char	main_c;

	find_main_c(vars, input_str);
	if (!creat_out_str(vars, input_str, &vars->out_str))
		return ;
	i = -1;
	main_c = 0;
	while (*input_str)
	{
		if (*input_str == vars->main_c)
		{
			if (--vars->q_count % 2 == 0 && main_c && vars->main_c == main_c)
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
}
