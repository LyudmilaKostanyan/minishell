/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 17:16:34 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/12 18:35:45 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_setlvl(const char *name, char **env, char *str)
{
	char	**temp;
	char	*line;
	int		i;

	i = 0;
	temp = env;
	while (temp[i] != NULL && ft_strncmp(name, temp[i], 5))
		++i;
	if (!ft_strncmp(name, temp[i], 5))
	{
		line = ft_substr(temp[i], 0, 6);
		temp[i] = ft_strjoin(line, str);
		free(str);
		return (0);
	}
	return (-1);
}

void	add_remove_shlvl(t_vars *var, int i)
{
	char	*shlvl;
	char	*newlvl;
	int		lvl;

	shlvl = getenv("SHLVL");
	newlvl = NULL;
	if (shlvl == NULL)
		ft_setlvl("SHLVL", var.env, "1");
	else if (i == 1)
	{
		lvl = ft_atoi(shlvl);
		++lvl;
		newlvl = ft_itoa(lvl);
	}
	else
	{
		lvl = ft_atoi(shlvl);
		--lvl;
		newlvl = ft_itoa(lvl);
	}
	ft_setlvl("SHLVL", var.env, newlvl);
}
