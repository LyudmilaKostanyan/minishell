#include "minishell.h"

void	here_doc(char *end)
{
	char	*line;
	char	*s;
	int		fd;

	s = ft_strjoin(end, "\n");
	fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line || !ft_strncmp(line, s, ft_strlen(line)))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
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
		
		here_doc((*cmds)[i].red_in);
		stop_program(dup2(open("tmp", O_RDONLY), 0) == -1, "", IO, vars->exit_stat);
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
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1, "", IO, vars->exit_stat);
		close((*cmds)[i].pipe[0]);
	}
	else if (count -1 && i == count - 1 && !(*cmds)[i].red_in && count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1, "", IO, vars->exit_stat);
		close((*cmds)[i - 1].pipe[1]);
	}
	else if (count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1, "", IO, vars->exit_stat);
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1, "", IO, vars->exit_stat);
	}
	return (redirection(vars, cmds, i));
}

void	pipes(t_vars *vars, t_cmds **cmds, int count)
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
