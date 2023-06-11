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

static int	size1(char const *s, char c)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != 0)
			k++;
		while (s[i] != c && s[i] != 0)
			i++;
	}
	return (k);
}

static int	mlc(char **str, int q, int j)
{
	if (q != 0)
	{
		str[j] = malloc(q + 1);
		if (str[j] == NULL)
		{
			while (--j > -1)
				free(str[j]);
			return (0);
		}
	}
	return (1);
}

static int	size2(char const *s, char c, char **str)
{
	int	i;
	int	j;
	int	q;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		q = 0;
		while (s[i] != c && s[i] && ++q)
			i++;
		if (!mlc(str, q, j))
			return (0);
		j++;
	}
	return (1);
}

static void	f(char const *s, char c, int k, char **s1)
{
	int	i;
	int	j;
	int	q;

	i = -1;
	j = 0;
	while (++i < k)
	{
		q = 0;
		while (s[j] == c && s[j] != 0)
			j++;
		while (s[j] != c && s[j] != 0)
		{
			s1[i][q] = s[j];
			j++;
			q++;
		}
		s1[i][q] = 0;
	}
	s1[i] = NULL;
}

char	**ft_split(char const *s, char c)
{
	int		k;
	int		q;
	char	**s1;

	k = size1(s, c);
	s1 = (char **)malloc(sizeof(char *) * (k + 1));
	if (s1 == NULL)
		return (NULL);
	q = size2(s, c, s1);
	if (q == 0)
	{
		free(s1);
		return (NULL);
	}
	f(s, c, k, s1);
	return (s1);
}
