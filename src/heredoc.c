/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:23:01 by tgalyaut          #+#    #+#             */
/*   Updated: 2023/09/19 19:23:02 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_init(t_vars *vars, char **line, char **out_str, int *i)
{
	t_mall_size	mall_size;

	*i = -1;
	mall_size.key_len = 0;
	mall_size.val_len = 0;
	if (!count_key_val(*vars, *line, &mall_size, 0))
		return (1);
	*out_str = malloc(ft_strlen(*line)
			- mall_size.key_len + mall_size.val_len + 1);
	return (0);
}

void	check_env_var(t_vars *vars, char **line)
{
	int		i;
	char	*out_str;
	char	*tmp;
	t_env	*env;

	if (check_init(vars, line, &out_str, &i))
		return ;
	tmp = *line;
	while (**line)
	{
		if (**line == '$')
		{
			env = find_same_key(*vars, *line);
			if (env)
				fill_out_str(line, &out_str, env, &i);
			else
				out_str[++i] = **line;
		}
		else
			out_str[++i] = **line;
		(*line)++;
	}
	out_str[++i] = 0;
	free(tmp);
	*line = out_str;
}

static void	join(t_vars *vars, char **str1, char *str2)
{
	char	*tmp;

	tmp = ft_strjoin(*str1, str2);
	malloc_err(!tmp, "here_doc", vars);
	if (*str1 && **str1)
		free(*str1);
	*str1 = tmp;
	tmp = NULL;
}

int	hd_while(t_vars *vars, char *end, int fd)
{
	char	*line;

	line = readline("> ");
	if (g_exit_status)
		return (0);
	if (!line || (*line && !ft_strcmp(line, end)))
	{
		free(line);
		return (0);
	}
	else if (!*line)
	{
		free(line);
		return (-1);
	}
	else
	{
		join(vars, &line, "\n");
		if (!vars->hd_stat)
			check_env_var(vars, &line);
		write(fd, line, ft_strlen(line));
		free(line);
	}
	return (1);
}

int	here_doc(t_vars *vars, char *end)
{
	int	*fds;
	int	fd;
	int	cond;

	g_exit_status = 0;
	fds = malloc(sizeof(int) * 2);
	malloc_err(!fds, "here_doc", vars);
	stop_program(pipe(fds) == -1, "", IO, vars);
	while (1)
	{
		vars->sig.sa_handler = &sig_handler;
		sigaction(SIGINT, &vars->sig, NULL);
		cond = hd_while(vars, end, fds[1]);
		if (!cond)
			break ;
		else if (cond == -1)
			continue ;
	}
	close(fds[1]);
	fd = fds[0];
	free(fds);
	return (fd);
}
