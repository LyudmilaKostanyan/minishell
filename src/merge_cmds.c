/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:23:19 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 19:23:20 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	red_in(t_cmds *cmds, char **sp_split, int j)
{
	if (!ft_strcmp(sp_split[j], "<") || !ft_strcmp(sp_split[j], "<<"))
	{
		if (!sp_split[j + 1] || !ft_strncmp(sp_split[j + 1], ">", 1)
			|| !ft_strncmp(sp_split[j + 1], ">>", 2)
			|| !ft_strncmp(sp_split[j + 1], "<", 1)
			|| !ft_strncmp(sp_split[j + 1], "<<", 2))
			cmds->in_stat = -1;
		else if (!ft_strcmp(sp_split[j], "<") && cmds->in_stat != -1)
			cmds->in_stat = 1;
		else if (cmds->in_stat != -1)
			cmds->in_stat = 2;
		if (sp_split[j + 1])
		{
			free(cmds->red_in);
			cmds->red_in = ft_strdup(sp_split[j + 1]);
			return (0);
		}
	}
	return (1);
}

void	red_out_if(t_cmds *cmds, t_vars *vars)
{
	int	t;

	if (cmds->red_out)
	{
		t = open(cmds->red_out, O_CREAT | O_WRONLY, 0644);
		err_mes(t == -1, join_err(vars, cmds->red_out, NULL), PD, vars);
		close(t);
	}
}

int	red_out(t_cmds *cmds, char **sp_split, int j, t_vars *vars)
{
	if (!ft_strcmp(sp_split[j], ">") || !ft_strcmp(sp_split[j], ">>"))
	{
		if (!sp_split[j + 1] || !ft_strncmp(sp_split[j + 1], ">", 1)
			|| !ft_strncmp(sp_split[j + 1], ">>", 2)
			|| !ft_strncmp(sp_split[j + 1], "<", 1)
			|| !ft_strncmp(sp_split[j + 1], "<<", 2))
			cmds->out_stat = -1;
		else if (!ft_strcmp(sp_split[j], ">") && cmds->out_stat != -1)
			cmds->out_stat = 1;
		else if (cmds->out_stat != -1)
			cmds->out_stat = 2;
		if (sp_split[j + 1])
		{
			red_out_if(cmds, vars);
			free(cmds->red_out);
			cmds->red_out = ft_strdup(sp_split[j + 1]);
			return (0);
		}
	}
	return (1);
}

void	redirections_init(t_vars *vars, t_cmds **cmds, char **sp_split, int i)
{
	int	k;
	int	j;

	k = -1;
	j = -1;
	while (sp_split[++j])
	{
		if (ft_strcmp(sp_split[j], ">") && ft_strcmp(sp_split[j], "<")
			&& ft_strcmp(sp_split[j], ">>") && ft_strcmp(sp_split[j], "<<"))
		{
			if (j == 0 || (ft_strcmp(sp_split[j - 1], ">")
					&& ft_strcmp(sp_split[j - 1], "<")
					&& ft_strcmp(sp_split[j - 1], ">>")
					&& ft_strcmp(sp_split[j - 1], "<<")))
			{
				(*cmds)[i].cmd[++k] = ft_strdup(sp_split[j]);
				malloc_err(!(*cmds)[i].cmd[k], CCL, vars);
			}
		}
		if (!red_in((*cmds) + i, sp_split, j))
			malloc_err(!(*cmds)[i].red_in, CRV, vars);
		if (!red_out((*cmds) + i, sp_split, j, vars))
			malloc_err(!(*cmds)[i].red_out, CRV, vars);
	}
	(*cmds)[i].cmd[++k] = NULL;
}

int	merge_cmds(t_vars *vars, t_cmds **cmds, char **pipe_splt, int count)
{
	int		i;
	int		j;
	char	**sp_split;

	i = -1;
	while (++i < count)
	{
		sp_split = splt_by_spaces(vars, cmds, pipe_splt, i);
		if (!sp_split)
			return (-1);
		(*cmds)[i].in_stat = 0;
		(*cmds)[i].out_stat = 0;
		(*cmds)[i].red_in = NULL;
		(*cmds)[i].red_out = NULL;
		(*cmds)[i].pipe = NULL;
		redirections_init(vars, cmds, sp_split, i);
		split_free(sp_split);
		j = -1;
		while ((*cmds)[i].cmd[++j])
			restore_spaces(&(*cmds)[i].cmd[j]);
		restore_spaces(&(*cmds)[i].red_in);
		restore_spaces(&(*cmds)[i].red_out);
	}
	split_free(pipe_splt);
	return (count);
}
