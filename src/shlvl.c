/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 17:16:34 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/12 19:34:10 by tgalyaut         ###   ########.fr       */
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
		free(line);
		free(str);
		return (0);
	}
	return (-1);
}

void	add_remove_shlvl(char **env, int i)
{
	char	*shlvl;
	char	*newlvl;
	int		lvl;

	shlvl = getenv("SHLVL");
	newlvl = NULL;
	if (shlvl == NULL)
		ft_setlvl("SHLVL", env, "1");
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
	ft_setlvl("SHLVL", env, newlvl);
}

char	*join_err(t_vars *vars, char *cmd, char *line)
{
	char	*str;
	char	*tmp;

	if (!line)
		str = ft_strdup(cmd);
	else
	{
		tmp = ft_strjoin(cmd, ": `");
		malloc_err(!tmp, "join_err", vars);
		str = tmp;
		tmp = ft_strjoin(str, line);
		malloc_err(!tmp, "join_err", vars);
		free(str);
		str = tmp;
		tmp = ft_strjoin(str, "'");
		free(str);
		str = tmp;
	}
	malloc_err(!str, "join_err", vars);
	return (str);
}

int	ft_intlen(int n)
{
	long	nb;
	int		len;

	len = 0;
	nb = n;
	if (nb < 0)
		nb = -nb;
	if (nb > 9)
		len += ft_intlen(nb / 10);
	len++;
	return (len);
}
