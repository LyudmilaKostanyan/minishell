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

char	*ft_strchr(const char *str, int c)
{
	int	i;

	i = -1;
	c %= 256;
	if (c == 0)
		return (&((char *)str)[ft_strlen(str)]);
	while (str[++i])
		if (str[i] == c)
			return (&((char *)str)[i]);
	return (NULL);
}
