/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_atoi.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lykostan <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/19 16:23:20 by lykostan		  #+#	#+#			 */
/*   Updated: 2023/01/19 16:23:23 by lykostan		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	f(const char *s1, const char *s2, size_t len, size_t j)
{
	size_t	i;

	i = 0;
	if (j + ft_strlen(s2) > len)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

char	*ft_strnstr(const char *s1, const char *s2, size_t len)
{
	size_t	i;

	if (!s1 && len == 0)
		return (NULL);
	if (!s2[0])
		return (((char *)s1));
	i = 0;
	while (s1[i] && i < len)
	{
		if (s1[i] == s2[0] && f((s1 + i), s2, len, i))
			return ((char *)s1 + i);
		i++;
	}
	return (NULL);
}
