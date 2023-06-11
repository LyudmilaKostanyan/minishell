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

static int	f(const char *str, int i)
{
	int	k;

	k = 0;
	while (str[i] == 48)
		i++;
	while (str[i] >= 48 && str[i] <= 57)
	{
		k = k * 10 + (str[i] - 48);
		i++;
	}
	return (k);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	k;
	int	m;

	m = 0;
	k = 0;
	i = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == 43)
		i++;
	else if (str[i] == 45)
	{
		m++;
		i++;
	}
	k = f(str, i);
	if (m != 0)
		return (-k);
	return (k);
}
