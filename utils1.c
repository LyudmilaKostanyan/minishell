/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:15:41 by lykostan          #+#    #+#             */
/*   Updated: 2023/07/31 23:56:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isalnum_str(char *str, char c)
{
	int	i;

	i = -1;
	while (str && str[++i])
	{
		if (c == 'e' && (ft_isalnum(str[i]) || str[i] == '_'
				|| (i == (int)ft_strlen(str) - 1 && str[i] == '+')))
			continue ;
		else if (c == 'u' && (ft_isalnum(str[i]) || str[i] == '_'))
			continue ;
		else if (ft_isalnum(str[i]))
			continue ;
		else
			return (0);
	}
	return (1);
}

long long	ft_atoll(char *str)
{
	unsigned long long	u_num;
	long long			ll_num;
	int					sign;

	sign = 1;
	u_num = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str == 48)
		str++;
	stop_program(ft_strlen(str) > 19, "export", EXIT_ERR, 255);
	while (*str)
	{
		stop_program(*str < 48 || *str > 57, "export", EXIT_ERR, 255);
		u_num = u_num * 10 + (*str++ - 48);
	}
	stop_program((sign == 1 && u_num > LLONG_MAX)
		|| (sign == -1 && u_num > (unsigned long long)(LLONG_MAX) + 1),
		"export", EXIT_ERR, 255);
	ll_num = sign * u_num;
	return (ll_num);
}

void	restore_spaces(char **str)
{
	int	i;

	i = -1;
	while ((*str)[++i])
		if ((*str)[i] == 1)
			(*str)[i] = 32;
}
