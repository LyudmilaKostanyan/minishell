/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:15:41 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/14 19:15:43 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <libft.h>

void	split_free(char **split)
{
	int	i;

	i = -1;
	while (split[++i])
		free(split[i]);
	free(split);
	split = NULL;
}

void	stop_program(int condition, char *cmd, char *issue, int exit_stat)
{
	if (condition)
	{
		printf("minishell: %s: %s\n", cmd, issue);
		exit(exit_stat);
	}
}

char	*tolower_str(char **str)
{
	int	i;

	i = -1;
	while (*str && (*str)[++i])
			(*str)[i] = ft_tolower((*str)[i]);
	return (*str);
}

int	ft_isalnum_str(char *str)
{
	int	i;

	i = -1;
	while (str && str[++i])
		if (!ft_isalnum(str[i]))
			return (0);
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
		|| (sign == -1 && u_num > (unsigned long long)(-LLONG_MIN)),
		"export", EXIT_ERR, 255);
	ll_num = sign * u_num;
	return (ll_num);
}
