/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:15:41 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 18:55:54 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	split_size(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		;
	return (i);
}

void	split_free(char **split)
{
	int	i;

	i = -1;
	while (split && split[++i])
		free(split[i]);
	free(split);
	split = NULL;
}

char	*tolower_str(char *str)
{
	int	i;

	i = -1;
	while (str && str[++i])
		str[i] = ft_tolower(str[i]);
	return (str);
}

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

long long	ft_atoll(t_vars *vars, char *str)
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
	stop_program(ft_strlen(str) > 19, "exit", EXIT_ERR, vars);
	while (*str)
	{
		stop_program(*str < 48 || *str > 57, "exit", EXIT_ERR, vars);
		u_num = u_num * 10 + (*str++ - 48);
	}
	stop_program((sign == 1 && u_num > LLONG_MAX)
		|| (sign == -1 && u_num > (unsigned long long)(LLONG_MAX) + 1),
		"exit", EXIT_ERR, vars);
	ll_num = sign * u_num;
	return (ll_num);
}
