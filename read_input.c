#include "minishell.h"

int	merge_cmds(t_cmds **cmds, char **pipe_splt, char **input_str)
{
	int		count;
	int		i;
	int		j;
	char	**sp_split;
	int		cmds_count;

	restore_spaces(input_str);
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
		{
			split_free(sp_split);
			split_free(pipe_splt);
			free(*cmds);
			return (-1);
		}

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
					(*cmds)[i].red_out = ft_strdup(sp_split[j + 1]);
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
					(*cmds)[i].red_in = ft_strdup(sp_split[j + 1]);
					malloc_err(!(*cmds)[i].red_in, "creating redirection vars");
				}
			}
		}
		(*cmds)[i].cmd[++k] = NULL;
		split_free(sp_split);

		j = -1;
		while ((*cmds)[i].cmd[++j])
			restore_spaces(&(*cmds)[i].cmd[j]);
		restore_spaces(&(*cmds)[i].red_in);
		restore_spaces(&(*cmds)[i].red_out);
	}
	split_free(pipe_splt);
	return (count);
}

int	check_pipes(char *input_str)
{
	int	i;
	int	len;

	len = ft_strlen(input_str);
	i = -1;
	while (input_str[++i] == 32)
		;
	if (input_str[i] && input_str[i] == '|')
		return (0);
	while (++i < len)
	{
		if (input_str[i] == '|')
		{
			while (input_str[++i] == 32)
				;
			if (input_str[i] == '|' || !input_str[i])
				return (0);
		}
	}
	return (1);
}

int	count_pipes(char *input_str)
{
	int	count;

	count = 0;
	while (*input_str)
		if (*(input_str++) == '|')
			count++;
	return (count);
}

int	read_input(t_vars *vars, t_cmds **cmds)
{
	char	*input_str;
	char	**pipe_splt;
	char	*for_split;
	int		count;

	pipe_splt = NULL;
	*cmds = NULL;
	input_str = readline("\e[34mminishell$ \e[0m");
	stop_program(!input_str, NULL, "exit");
	if (!*input_str)
	{
		free(input_str);
		return (-1);
	}
	if (!check_pipes(input_str))
	{
		add_history(input_str);
		free(input_str);
		return (0);
	}
	if (count_pipes(input_str) > 123)		//123????????
	{
		add_history(input_str);
		free(input_str);
		return (-2);
	}
	quotes_handler(vars, &input_str);
	split_free(pipe_splt);
	for_split = rm_quotes(vars, input_str);
	if (!for_split)
		pipe_splt = ft_split(input_str, '|');
	else
		pipe_splt = ft_split(for_split, '|');
	free(for_split);
	malloc_err(!pipe_splt, "split cmds");
	count = merge_cmds(cmds, pipe_splt, &input_str);
	add_history(input_str);
	free(input_str);
	return (count);
}
