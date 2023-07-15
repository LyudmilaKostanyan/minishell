#include "minishell.h"

int	merge_cmds(t_cmds **cmds, char **pipe_splt, char **input_str)
{
	int		count;
	int		i;
	int		j;
	char	**sp_split;
	int		cmds_count;

	restore_spaces(input_str);
	add_history(*input_str);
	count = split_size(pipe_splt);
	free(*cmds);
	*cmds = malloc((sizeof(t_cmds) * count) + 1);
	malloc_err(!*cmds, "creat cmds");
	i = -1;
	while (++i < count)
	{
		sp_split = ft_split(pipe_splt[i], ' ');
		malloc_err(!sp_split, "creating cmd list");
		if (!*sp_split)
			return (-1);

		j = -1;
		cmds_count = 0;
		while (sp_split[++j])
			if (ft_strcmp(sp_split[j], ">") && ft_strcmp(sp_split[j], "<")
				&& ft_strcmp(sp_split[j], ">>") && ft_strcmp(sp_split[j], "<<"))
				if (j == 0 || (ft_strcmp(sp_split[j - 1], ">") && ft_strcmp(sp_split[j - 1], "<")
					&& ft_strcmp(sp_split[j - 1], ">>") && ft_strcmp(sp_split[j - 1], "<<")))
					cmds_count++;
		(*cmds)[i].cmd = malloc(sizeof(char *) * (cmds_count + 1));
		malloc_err(!(*cmds)[i].cmd, "creating cmd list");
		j = -1;
		int	k = -1;
		(*cmds)[i].in_stat = 0;
		(*cmds)[i].out_stat = 0;
		(*cmds)[i].red_in = NULL;
		(*cmds)[i].red_out = NULL;
		while (sp_split[++j])
		{
			if (ft_strcmp(sp_split[j], ">") && ft_strcmp(sp_split[j], "<")
				&& ft_strcmp(sp_split[j], ">>") && ft_strcmp(sp_split[j], "<<"))
			{
				if (j == 0 || (ft_strcmp(sp_split[j - 1], ">") && ft_strcmp(sp_split[j - 1], "<")
					&& ft_strcmp(sp_split[j - 1], ">>") && ft_strcmp(sp_split[j - 1], "<<")))
				{
					(*cmds)[i].cmd[++k] = ft_strdup(sp_split[j]);
					malloc_err(!(*cmds)[i].cmd[k], "creating cmd list");
				}
			}
			else if (!ft_strcmp(sp_split[j], ">") || !ft_strcmp(sp_split[j], ">>"))
			{
				if (!ft_strcmp(sp_split[j], ">"))
					(*cmds)[i].out_stat = 1;
				else
					(*cmds)[i].out_stat = 2;
				if (sp_split[j + 1])
				{
					(*cmds)[i].red_out = ft_strdup(sp_split[j + 1]);		//error
					malloc_err(!(*cmds)[i].red_out, "creating redirection vars");
				}
			}
			else if (!ft_strcmp(sp_split[j], "<") || !ft_strcmp(sp_split[j], "<<"))
			{
				if (!ft_strcmp(sp_split[j], "<"))
					(*cmds)[i].in_stat = 1;
				else
					(*cmds)[i].in_stat = 2;
				if (sp_split[j + 1])
				{
					(*cmds)[i].red_in = ft_strdup(sp_split[j + 1]);		//error
					malloc_err(!(*cmds)[i].red_in, "creating redirection vars");
				}
			}
		}
		(*cmds)[i].cmd[++k] = NULL;
		split_free(sp_split);

		j = -1;
		while ((*cmds)[i].cmd[++j])
			restore_spaces(&(*cmds)[i].cmd[j]);
	}
	split_free(pipe_splt);
	return (count);
}

int	read_input(t_vars *vars, t_cmds **cmds)
{
	char	*input_str;
	char	**pipe_splt;

	input_str = NULL;
	pipe_splt = NULL;
	*cmds = NULL;
	free(input_str);
	input_str = readline("\e[34mminishell$ \e[0m");
	stop_program(!input_str, NULL, "exit", vars->exit_stat);
	if (!*input_str)
		return (-1);
	quotes_handler(vars, &input_str);
	char *for_split;
	split_free(pipe_splt);
	for_split = rm_quotes(vars, input_str);
	if (!for_split)
		pipe_splt = ft_split(input_str, '|');
	else
		pipe_splt = ft_split(for_split, '|');
	free(for_split);
	malloc_err(!pipe_splt, "split cmds");
	if (err_mes(!*pipe_splt, vars, NULL, PIPE_ERR))
		return (0);
	return (merge_cmds(cmds, pipe_splt, &input_str));
}
