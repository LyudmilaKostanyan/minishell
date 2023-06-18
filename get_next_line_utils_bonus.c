/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 17:38:06 by lykostan          #+#    #+#             */
/*   Updated: 2023/02/01 17:38:08 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strchr_gnl(char *str, int c)
{
	int	i;

	i = -1;
	c %= 256;
	if (c == 0)
		return (ft_strlen(str) - 1);
	while (str[++i])
		if (str[i] == c)
			return (i);
	return (-1);
}

char	*ft_strdup_gnl(char *s, int start, int j)
{
	char	*str;
	int		i;

	str = malloc(j + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < j)
	{
		str[i] = s[start];
		i++;
		start++;
	}
	str[i] = 0;
	return (str);
}

char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;
	int		j;
	int		len1;
	int		len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	str = malloc(len1 + len2 + 1);
	if (str == NULL)
		return (NULL);
	j = -1;
	while (++j < len1)
		str[j] = s1[j];
	j = -1;
	while (++j < len2)
		str[len1++] = s2[j];
	str[len1] = 0;
	return (str);
}
