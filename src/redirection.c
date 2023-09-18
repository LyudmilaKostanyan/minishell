#include "minishell.h"

int	redirection(t_vars *vars, t_cmds **cmds, int i)
{
	if ((*cmds)[i].in_stat == 1 && err_mes(dup2(open((*cmds)[i].red_in,
		O_RDONLY), 0) == -1, (*cmds)[i].red_in, NULL, CD_ERR))
		return (0);
	else if ((*cmds)[i].in_stat == 2)
		stop_program(dup2(here_doc(vars, (*cmds)[i].red_in), 0) == -1, "",
			IO, vars->true_env);
	if ((*cmds)[i].out_stat == 1)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_TRUNC
					| O_WRONLY, 0644), 1) == -1, "", IO, vars->true_env);
	else if ((*cmds)[i].out_stat == 2)
		stop_program(dup2(open((*cmds)[i].red_out, O_CREAT | O_APPEND
					| O_WRONLY, 0644), 1) == -1, "", IO, vars->true_env);
	return (1);
}

int	redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i)
{
	if (i == 0 && !(*cmds)[i].red_out && count != 1)
	{
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1,
			"", IO, vars->true_env);
		close((*cmds)[i].pipe[0]);
	}
	else if (i == count - 1 && !(*cmds)[i].red_in && count != 1)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1,
			"", IO, vars->true_env);
		close((*cmds)[i - 1].pipe[1]);
	}
	else if (count != 1 && !(*cmds)[i].red_in)
	{
		stop_program(dup2((*cmds)[i - 1].pipe[0], 0) == -1,
			"", IO, vars->true_env);
		stop_program(dup2((*cmds)[i].pipe[1], 1) == -1,
			"", IO, vars->true_env);
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
			stop_program(1, "", "creat pipes", vars->true_env);
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
