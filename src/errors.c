/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 18:49:05 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/15 18:49:07 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_err(int condition, char *cmd, t_vars *vars)
{
	if (condition)
	{
		write(vars->fd_out, "minishell: malloc error: ", 25);
		write(vars->fd_out, cmd, ft_strlen(cmd));
		write(vars->fd_out, "\n", 1);
		add_remove_shlvl(vars->true_env, -42);
		exit(1);
	}
}

static void	write_program(char *cmd, char *issue, t_vars *vars)
{
	if (!cmd && !ft_strcmp(issue, "exit"))
	{
		write(vars->fd_out, issue, ft_strlen(issue));
		write(vars->fd_out, "\n", 1);
	}
	else if (!cmd)
	{
		write(vars->fd_out, "minishell: ", 11);
		write(vars->fd_out, issue, ft_strlen(issue));
		write(vars->fd_out, "\n", 1);
	}
	else
	{
		write(vars->fd_out, "minishell: ", 11);
		write(vars->fd_out, cmd, ft_strlen(cmd));
		write(vars->fd_out, ": ", 2);
		write(vars->fd_out, issue, ft_strlen(issue));
		write(vars->fd_out, "\n", 1);
	}
}

void	stop_program(int condition, char *cmd, char *issue, t_vars *vars)
{
	if (condition)
	{
		write_program(cmd, issue, vars);
		if (!ft_strcmp(cmd, "exit"))
		{
			add_remove_shlvl(vars->true_env, -42);
			exit(255);
		}
		add_remove_shlvl(vars->true_env, -42);
		exit(g_exit_status);
	}
}

int	err_mes(int condition, char *cmd, char *line, char *issue)
{
	if (condition)
	{
		if (!cmd && !line)
			printf("minishell: %s\n", issue);
		else if (!line)
			printf("minishell: %s: %s\n", cmd, issue);
		else
			printf("minishell: %s: `%s': %s\n", cmd, line, issue);
		g_exit_status = 1;
		return (1);
	}
	return (0);
}
