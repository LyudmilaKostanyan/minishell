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

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*s1;
	unsigned char	*s2;
	size_t			nb;

	if (!dest && !src)
		return (NULL);
	s1 = (unsigned char *)dest;
	s2 = (unsigned char *)src;
	nb = n;
	if (s1 > s2)
	{
		s1 += n;
		s2 += n;
		while (n--)
			*--s1 = *--s2;
	}
	else
	{
		while (n--)
			*s1++ = *s2++;
		return (s1 - nb);
	}
	return (s1);
}
