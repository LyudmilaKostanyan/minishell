/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 23:49:43 by lykostan          #+#    #+#             */
/*   Updated: 2023/08/01 21:23:17 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	split_size(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		;
	return (i);
}

void	split_free(char **split)
{
	int	i;

	i = -1;
	while (split && split[++i])
		free(split[i]);
	free(split);
	split = NULL;
}

char	*tolower_str(char *str)
{
	int	i;

	i = -1;
	while (str && str[++i])
			str[i] = ft_tolower(str[i]);
	return (str);
}
