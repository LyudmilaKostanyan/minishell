/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_calloc.c										:+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: lykostan <marvin@42.fr>					+#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2023/01/19 16:33:47 by lykostan		  #+#	#+#			 */
/*   Updated: 2023/01/19 16:33:48 by lykostan		 ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*str;
	size_t	i;

	if (nitems == SIZE_MAX || size == SIZE_MAX)
		return (NULL);
	str = malloc(nitems * size);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (i < nitems * size)
		((char *)str)[i++] = 0;
	return (str);
}
