/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:27 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 19:32:29 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo_check_args(char **cmd)
{
	int	i;
	int	j;

	i = 0;
	while (cmd[++i])
	{
		j = 0;
		if (cmd[i][j] != '-')
			break ;
		while (cmd[i][++j])
			if (cmd[i][j] != 'n')
				break ;
		if (j != (int)ft_strlen(cmd[i]))
			break ;
	}
	return (i);
}

void	echo(char **cmd)
{
	int		i;
	int		cond;

	i = echo_check_args(cmd) - 1;
	cond = i + 1;
	while (cmd[++i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
	}
	if (cond == 1)
		printf("\n");
	g_exit_status = 0;
}

void	env(t_vars *vars, int cmd)
{
	t_env	*tmp;

	tmp = vars->env;
	while (tmp)
	{
		if (cmd)
			printf("%s\n", tmp->line);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	g_exit_status = 0;
}

void	unset(t_vars *vars, char **cmd)
{
	int		i;

	i = 0;
	g_exit_status = 0;
	while (cmd[++i])
	{
		if (!ft_isdigit(*cmd[i]) && ft_isalnum_str(cmd[i], 'u'))
			find_node(vars, cmd[i]);
		else
			err_mes(1, join_err(vars, *cmd, cmd[i]), E_U_ERR, vars);
	}
}

void	exit_prog(t_vars *vars, char **cmd)
{
	long long	exit_code;

	if (err_mes(cmd[1] && cmd[2] != NULL,
			join_err(vars, *cmd, NULL), TMA, vars))
		return ;
	if (cmd[1])
	{
		exit_code = ft_atoll(vars, cmd[1]);
		exit(exit_code % 256);
	}
	else
		exit(0);
}
