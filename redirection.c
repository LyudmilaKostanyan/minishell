/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/31 22:46:27 by lykostan          #+#    #+#             */
/*   Updated: 2023/07/31 23:56:27 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*qwe(t_vars *vars, char *line)
{
	char	*key;
	t_env	*env;

	if (line[ft_strlen(line) - 1] == '\n')
		key = ft_substr(line, 1, ft_strlen(line) - 2);
	else
		key = ft_substr(line, 1, ft_strlen(line) - 1);
	env = find_key(*vars, key);
	free(key);
	return (env);
}

int	asd(t_vars *vars, char *input_str, t_mall_size *mall_size)
{
	t_env	*env;
	int		cond;

	cond = 0;
	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
		{
			cond++;
			env = qwe(vars, input_str);
			if (env)
			{
				mall_size->key_len += ft_strlen(env->key) + 1;
				mall_size->val_len += ft_strlen(env->value);
			}
		}
		input_str++;
	}
	return (cond);
}

static char	*cev_init(t_vars *vars, char **line)
{
	char		*out_str;
	t_mall_size	mall_size;

	mall_size.key_len = 0;
	mall_size.val_len = 0;
	asd(vars, *line, &mall_size);
	out_str = malloc(ft_strlen(*line) - mall_size.key_len
			+ mall_size.val_len + 2);
	return (out_str);
}

static char	*cev_help(t_vars *vars, char **line, t_env *env, int *cond)
{
	char	*out_str;
	int		i;

	out_str = cev_init(vars, line);
	i = -1;
	while (**line)
	{
		if (**line == '$')
		{
			cond++;
			env = qwe(vars, *line);
			if (env)
				fill_out_str(line, &out_str, env, &i);
			else
				out_str[++i] = **line;
		}
		else
			out_str[++i] = **line;
		(*line)++;
	}
	if (cond)
		out_str[++i] = '\n';
	out_str[++i] = 0;
	return (out_str);
}

void	check_env_var(t_vars *vars, char **line)
{
	t_env		env;
	int			cond;

	cond = 0;
	*line = cev_help(vars, line, &env, &cond);
}

void	here_doc(t_vars *vars, char *end)
{
	char	*line;
	char	*s;
	int		fd;

	s = ft_strjoin(end, "\n");
	fd = open("tmp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	while (1)
	{
		write(vars->fd_out, "> ", 2);
		line = get_next_line(0);
		if (!line || !ft_strncmp(line, s, ft_strlen(line)))
		{
			free(line);
			break ;
		}
		else
		{
			check_env_var(vars, &line);
			write(fd, line, ft_strlen(line));
			free(line);
		}
	}
	close(fd);
	free(s);
}

int	redirection(t_vars *vars, t_cmds **cmds, int i)
{
	if ((*cmds)[i].in_stat == 1 && err_mes(dup2(open((*cmds)[i].red_in,		//meh
		O_RDONLY), 0) == -1, vars, NULL, CD_ERR))
		return (0);
	else if ((*cmds)[i].in_stat == 2)
	{
		here_doc(vars, (*cmds)[i].red_in);
		stop_program(dup2(open("tmp", O_RDONLY), 0)
			== -1, "", IO, vars->exit_stat);
	}
	if ((*cmds)[i].out_stat == 1)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_TRUNC
					| O_WRONLY, 0644), 1) == -1, "", IO, vars->exit_stat);
	else if ((*cmds)[i].out_stat == 2)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_APPEND
					| O_WRONLY, 0644), 1) == -1, "", IO, vars->exit_stat);
	return (1);
}

int	redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i)
{
	if (i == 0 && !(*cmds)[i].red_out && count != 1)
	{
		stop_program(dup2((*cmds)[i].pipe[1], 1)
			== -1, "", IO, vars->exit_stat);
		close((*cmds)[i].pipe[0]);
	}
	else if (count -1 && i == count - 1 && !(*cmds)[i].red_in && count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0)
			== -1, "", IO, vars->exit_stat);
		close((*cmds)[i - 1].pipe[1]);
	}
	else if (count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0)
			== -1, "", IO, vars->exit_stat);
		stop_program(dup2((*cmds)[i].pipe[1], 1)
			== -1, "", IO, vars->exit_stat);
	}
	return (redirection(vars, cmds, i));
}

void	pipes(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count - 1)
	{
		(*cmds)[i].pipe = malloc(sizeof(int) * 2);
		if (!(*cmds)[i].pipe || pipe((*cmds)[i].pipe) == -1)
		{
			while (--i >= 0)
			{
				close((*cmds)[i].pipe[0]);
				close((*cmds)[i].pipe[1]);
			}
			stop_program(1, "", "creat pipes", vars->exit_stat);
		}
	}
}

void	close_pipes(t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count - 1)
	{
		close((*cmds)[i].pipe[0]);
		close((*cmds)[i].pipe[1]);
	}
}
