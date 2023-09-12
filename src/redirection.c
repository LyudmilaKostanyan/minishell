#include "minishell.h"

void	check_env_var(t_vars *vars, char **line)
{
	int			i;
	char		*out_str;
	char		*tmp;
	t_env		*env;
	t_mall_size	mall_size;

	mall_size.key_len = 0;
	mall_size.val_len = 0;
	if (!count_key_val(*vars, *line, &mall_size, 0))
		return ;
	out_str = malloc(ft_strlen(*line) - mall_size.key_len + mall_size.val_len + 1);
	i = -1;
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
	*line = tmp;
	out_str[++i] = 0;
	free(*line);
	*line = out_str;
}

void	join(char **str1, char *str2)
{
	char	*tmp;

	tmp = ft_strjoin(*str1, str2);
	malloc_err(!tmp, "here_doc");
	if (*str1 && **str1)
		free(*str1);
	*str1 = tmp;
	tmp = NULL;
}

void	hd_action(int signal)
{
	if (signal == SIGINT)
		g_exit_status = 1;
	rl_catch_signals = 0;
}

int	here_doc(t_vars *vars, char *end)
{
	char	*line;
	int		*fds;
	int					fd;
	struct sigaction	sig;
	sigset_t			sigset;

	sig.sa_handler = &hd_action;
	sig.sa_flags = SA_RESTART;
	sigaddset(&sigset, SIGINT);
	sigaction(SIGINT, &sig, NULL);
	fds = malloc(sizeof(int) * 2);
	malloc_err(!fds, "here_doc");
	stop_program(pipe(fds) == -1, "", IO);
	while (1)
	{
		line = readline("> ");
		if (!line || (*line && !ft_strcmp(line, end)))
		{
			free(line);
			break ;
		}
		else if (!*line)
		{
			free(line);
			continue ;
		}
		else
		{
			join(&line, "\n");
			if (!vars->hd_stat)
				check_env_var(vars, &line);
			write(fds[1], line, ft_strlen(line));
			free(line);
		}
	}
	close(fds[1]);
	fd = fds[0];
	free(fds);
	g_exit_status = 0;
	return (fd);
}

int	redirection(t_vars *vars, t_cmds **cmds, int i)
{
	if ((*cmds)[i].in_stat == 1 && err_mes(dup2(open((*cmds)[i].red_in,		//meh
		O_RDONLY), 0) == -1, (*cmds)[i].red_in, NULL, CD_ERR))
			return (0);
	else if ((*cmds)[i].in_stat == 2)
		stop_program(dup2(here_doc(vars, (*cmds)[i].red_in), 0) == -1, "", IO);
	if ((*cmds)[i].out_stat == 1)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_TRUNC
			| O_WRONLY, 0644), 1) == -1, "", IO);
	else if ((*cmds)[i].out_stat == 2)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_APPEND
			| O_WRONLY, 0644), 1) == -1, "", IO);
	return (1);
}

int	redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i)
{
	if (i == 0 && !(*cmds)[i].red_out && count != 1)
	{
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1, "", IO);
		close((*cmds)[i].pipe[0]);
	}
	else if (i == count - 1 && !(*cmds)[i].red_in && count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1, "", IO);
		close((*cmds)[i - 1].pipe[1]);
	}
	else if (count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1, "", IO);
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1, "", IO);
	}
	return (redirection(vars, cmds, i));
}

void	pipes(t_cmds **cmds, int count)
{
	int i;

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
			stop_program(1, "", "creat pipes");
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
