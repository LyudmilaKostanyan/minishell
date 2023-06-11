/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_itoa.c										  :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lykostan <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/19 16:35:05 by lykostan		  #+#	#+#			 */
/*   Updated: 2023/01/19 16:35:06 by lykostan		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	ft_pow(int i)
{
	int	q;

	q = 1;
	while (i)
	{
		q *= 10;
		i--;
	}
	return (q);
}

static int	f(int n)
{
	int	len;

	len = 0;
	while (n > 9 || n < -9)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

static void	f1(int i, int len, char *s, long nbr)
{
	while (++i != len)
		s[i] = ((nbr / ft_pow(len - i - 1)) % 10) + 48;
	s[i] = 0;
}

char	*ft_itoa(int nb)
{
	char	*s;
	int		len;
	int		i;
	long	nbr;

	i = -1;
	nbr = nb;
	len = f(nbr) + 1;
	if (nbr >= 0)
	{
		s = malloc(sizeof(char) * len + 1);
		if (s == NULL)
			return (NULL);
	}
	else
	{
		s = malloc(sizeof(char) * len++ + 2);
		if (s == NULL)
			return (NULL);
		s[++i] = '-';
		nbr = -nbr;
	}
	f1(i, len, s, nbr);
	return (s);
}
