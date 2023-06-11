/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 20:56:16 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/11 20:56:19 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	check_str(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

int	ft_strstr(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1)
		return (-1);
	if (!*s2)
		return (0);
	i = 0;
	while (s1[i])
	{
		if (s1[i] == *s2 && check_str((s1 + i), s2))
			return (i);
		i++;
	}
	return (-1);
}
