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

static int	f(char const s, char const *set2)
{
	int	i;

	i = -1;
	while (set2[++i])
		if (set2[i] == s)
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		k;
	char	*s;

	i = -1;
	while (s1[++i])
		if (!(f(s1[i], set)))
			break ;
	j = (int)ft_strlen(s1);
	while (--j > -1)
		if (!(f(s1[j], set)))
			break ;
	if (i == (int)ft_strlen(s1) && j == -1)
		return (ft_strdup(""));
	s = malloc(j - i + 2);
	if (s == NULL)
		return (NULL);
	k = -1;
	while (i < j + 1)
		s[++k] = s1[i++];
	s[++k] = '\0';
	return (s);
}
