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

char	**splt_by_spaces(t_vars *vars, t_cmds **cmds, char **pipe_splt, int i)
{
	char	**sp_split;
	int		j;
	int		cmds_count;

	sp_split = ft_split(pipe_splt[i], ' ');
	malloc_err(!sp_split, "creating cmd list", vars);
	if (!*sp_split)
	{
		split_free(sp_split);
		split_free(pipe_splt);
		return (NULL);
	}
	j = -1;
	cmds_count = 0;
	while (sp_split[++j])
		if (ft_strcmp(sp_split[j], ">") && ft_strcmp(sp_split[j], "<")
			&& ft_strcmp(sp_split[j], ">>") && ft_strcmp(sp_split[j], "<<"))
			if (j == 0 || (ft_strcmp(sp_split[j - 1], ">") && ft_strcmp(
						sp_split[j - 1], "<") && ft_strcmp(sp_split[j - 1],
						">>") && ft_strcmp(sp_split[j - 1], "<<")))
				cmds_count++;
	(*cmds)[i].cmd = malloc(sizeof(char *) * (cmds_count + 1));
	malloc_err(!(*cmds)[i].cmd, "creating cmd list", vars);
	return (sp_split);
}

int	red_in(t_cmds **cmds, char **sp_split, int i, int j)
{
	if (!ft_strcmp(sp_split[j], "<") || !ft_strcmp(sp_split[j], "<<"))
	{
		if (!sp_split[j + 1] || !ft_strncmp(sp_split[j + 1], ">", 1)
			|| !ft_strncmp(sp_split[j + 1], ">>", 2)
			|| !ft_strncmp(sp_split[j + 1], "<", 1)
			|| !ft_strncmp(sp_split[j + 1], "<<", 2) || (*cmds)[i].in_stat)
			(*cmds)[i].in_stat = -1;
		else if (!ft_strcmp(sp_split[j], "<") && (*cmds)[i].in_stat != -1)
			(*cmds)[i].in_stat = 1;
		else if ((*cmds)[i].in_stat != -1)
			(*cmds)[i].in_stat = 2;
		if (sp_split[j + 1])
		{
			(*cmds)[i].red_in = ft_strdup(sp_split[j + 1]);
			return (0);
		}
	}
	return (1);
}

int	red_out(t_cmds **cmds, char **sp_split, int i, int j)
{
	if (!ft_strcmp(sp_split[j], ">") || !ft_strcmp(sp_split[j], ">>"))
	{
		if (!sp_split[j + 1] || !ft_strncmp(sp_split[j + 1], ">", 1)
			|| !ft_strncmp(sp_split[j + 1], ">>", 2)
			|| !ft_strncmp(sp_split[j + 1], "<", 1)
			|| !ft_strncmp(sp_split[j + 1], "<<", 2) || (*cmds)[i].out_stat)
			(*cmds)[i].out_stat = -1;
		else if (!ft_strcmp(sp_split[j], ">") && (*cmds)[i].out_stat != -1)
			(*cmds)[i].out_stat = 1;
		else if ((*cmds)[i].out_stat != -1)
			(*cmds)[i].out_stat = 2;
		if (sp_split[j + 1])
		{
			(*cmds)[i].red_out = ft_strdup(sp_split[j + 1]);
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
		if (!red_in(cmds, sp_split, i, j))
			malloc_err(!(*cmds)[i].red_in, CRV, vars);
		if (!red_out(cmds, sp_split, i, j))
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
