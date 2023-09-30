/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:23:30 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 19:23:31 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	redirection_out(t_vars *vars, t_cmds **cmds, int i)
{
	int	fd;

	if ((*cmds)[i].out_stat == 1)
	{
		fd = open((*cmds)[i].red_out, O_CREAT | O_TRUNC | O_WRONLY, 0644);
		stop_program(dup2(fd, 1) == -1, NULL, IO, vars);
		close(fd);
	}
	else if ((*cmds)[i].out_stat == 2)
	{
		fd = open((*cmds)[i].red_out, O_CREAT | O_APPEND | O_WRONLY, 0644);
		stop_program(dup2(fd, 1) == -1, NULL, IO, vars);
		close(fd);
	}
}

int	redirection_in(t_vars *vars, t_cmds **cmds, int i)
{
	int	fd;

	if ((*cmds)[i].in_stat == 1)
	{
		fd = open((*cmds)[i].red_in, O_RDONLY);
		if (err_mes(dup2(fd, 0) == -1, join_err(vars,
					(*cmds)[i].red_in, NULL), CD_ERR, vars))
			return (0);
		close(fd);
	}
	else if ((*cmds)[i].in_stat == 2)
	{
		fd = here_doc(vars, (*cmds)[i].red_in);
		if (fd == -1)
			return (0);
		stop_program(dup2(fd, 0) == -1, NULL, IO, vars);
		close(fd);
	}
	redirection_out(vars, cmds, i);
	return (1);
}

int	redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i)
{
	int	cond;

	cond = redirection_in(vars, cmds, i);
	if (i == 0 && !(*cmds)[i].red_out && count != 1)
	{
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1,
			NULL, IO, vars);
		close((*cmds)[i].pipe[0]);
	}
	else if (i == count - 1 && !(*cmds)[i].red_in && count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1,
			NULL, IO, vars);
		close((*cmds)[i - 1].pipe[1]);
	}
	else if (count != 1 && !(*cmds)[i].red_in && !(*cmds)[i].red_out)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1,
			NULL, IO, vars);
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1,
			NULL, IO, vars);
	}
	return (cond);
}

void	pipes(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count - 1)
	{
		(*cmds)[i].pipe = malloc(sizeof(int) * 2);
		malloc_err(!(*cmds)[i].pipe, "creating pipes", vars);
		if (!(*cmds)[i].pipe || pipe((*cmds)[i].pipe) == -1)
		{
			while (--i >= 0)
			{
				close((*cmds)[i].pipe[0]);
				close((*cmds)[i].pipe[1]);
			}
			stop_program(1, NULL, "creat pipes", vars);
		}
	}
}

void	close_pipes(t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count - 1)
	{
		close((*cmds)[i].pipe[0]);
		close((*cmds)[i].pipe[1]);
	}
}
