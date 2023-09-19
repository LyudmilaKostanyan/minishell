/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 20:11:09 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 20:11:10 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	action(int signal)
{
	if (signal == SIGINT)
		g_exit_status = 1;
	rl_replace_line("", 0);
	rl_done = 1;
}

int	empty(void)
{
	return (0);
}

int	check_redirection(t_cmds *cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		if (err_mes(cmds[i].in_stat == -1
				|| cmds[i].out_stat == -1, NULL, NULL, "syntax error"))
		{
			g_exit_status = 258;
			return (0);
		}
	}
	return (1);
}
