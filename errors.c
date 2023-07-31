/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 22:33:14 by lykostan          #+#    #+#             */
/*   Updated: 2023/07/31 23:56:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	malloc_err(int condition, char *cmd)
{
	if (condition)
	{
		printf("minishell: malloc error: %s\n", cmd);
		exit(1);
	}
}

void	stop_program(int condition, char *cmd, char *issue, int exit_stat)
{
	if (condition)
	{
		if (!cmd && !ft_strcmp(issue, "exit"))
			printf("%s\n", issue);
		else if (!cmd)
			printf("minishell: %s\n", issue);
		else
			printf("minishell: %s: %s\n", cmd, issue);
		exit(exit_stat);
	}
}

int	err_mes(int condition, t_vars *vars, char **cmd, char *line)
{
	if (condition)
	{
		if (!cmd)
			printf("minishell: %s\n", line);
		else if (*cmd[0] == 'u' || *cmd[0] == 'e')
			printf("minishell: %s: `%s': %s\n", cmd[0], line, E_U_ERR);
		else if (*cmd[0] == 'c')
		{
			if (!cmd[2])
				printf("minishell: %s: `%s': %s\n", cmd[0], line, CD_ERR);
			else
				printf("minishell: %s: %s\n", cmd[0], TMA);
		}
		vars->exit_stat = 1;		//error
		return (1);
	}
	return (0);
}
