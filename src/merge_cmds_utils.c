/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_cmds_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:20:07 by lykostan          #+#    #+#             */
/*   Updated: 2023/10/03 18:20:56 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	merge_cmds_init(t_cmds **cmds, int i)
{
	(*cmds)[i].in_stat = 0;
	(*cmds)[i].out_stat = 0;
	(*cmds)[i].red_in = NULL;
	(*cmds)[i].red_out = NULL;
	(*cmds)[i].pipe = NULL;
	(*cmds)[i].cmd = NULL;
}

void	merge_cmds_rs(t_cmds **cmds, int i)
{
	int	j;

	j = -1;
	while ((*cmds)[i].cmd[++j])
		restore_spaces(&(*cmds)[i].cmd[j]);
	restore_spaces(&(*cmds)[i].red_in);
	restore_spaces(&(*cmds)[i].red_out);
}

int	red_if_cmd(t_vars *vars, char **cmd, char **sp_split, int j)
{
	if (j == 0 || (ft_strcmp(sp_split[j - 1], ">")
			&& ft_strcmp(sp_split[j - 1], "<")
			&& ft_strcmp(sp_split[j - 1], ">>")
			&& ft_strcmp(sp_split[j - 1], "<<")))
	{
		*cmd = ft_strdup(sp_split[j]);
		malloc_err(!*cmd, CCL, vars);
		return (1);
	}
	return (0);
}
