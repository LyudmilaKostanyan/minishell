/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:51 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/14 19:17:54 by lykostan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env *creat_env_list(char **env)
{
	t_env *head;
	t_env *node;
	int equal;

	head = malloc(sizeof(t_env));
	malloc_err(!head, "env list");
	node = head;
	while (*env)
	{
		equal = ft_strchr(*env, '=') - *env;
		node->line = ft_strdup(*env);
		node->key = ft_substr(*env, 0, equal);
		node->value = ft_substr(*env, equal + 1, ft_strlen(*env) - equal);
		malloc_err(!node->line || !node->key || !node->line, "env list");
		if (!*(env + 1))
			node->next = NULL;
		else
		{
			node->next = malloc(sizeof(t_env));
			malloc_err(!node->next, "env list");
		}
		node = node->next;
		env++;
	}
	return (head);
}

int check_builtins(t_vars *vars, char **cmd)
{
	if (!ft_strcmp(*cmd, "pwd"))
		pwd(vars);
	else if (!ft_strcmp(*cmd, "cd"))
		cd(vars, cmd);
	else if (!ft_strcmp(*cmd, "env"))
		env(vars, 1);
	else if (!ft_strcmp(*cmd, "echo"))
		echo(vars, cmd);
	else if (!ft_strcmp(*cmd, "export"))
		export(vars, cmd);
	else if (!ft_strcmp(*cmd, "unset"))
		unset(vars, cmd);
	else if (!ft_strcmp(*cmd, "exit"))
		exit_prog(cmd);
	else
		return (0);
	return (1);
}

void restore_spaces(char **str)
{
	int i;

	i = -1;
	while ((*str)[++i])
		if ((*str)[i] == 1)
			(*str)[i] = 32;
}

int check_equal(t_vars *vars, char **cmd)
{
	int			i;
	long long	equal;
	char		*key;
	int			cond;

	i = -1;
	cond = 0;
	while (cmd[++i])
	{
		equal = ft_strchr(cmd[i], '=') - cmd[i];
		key = ft_substr(cmd[i], 0, equal);
		malloc_err(!key, cmd[0]);
		if (equal >= 0 && !ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e') && ++cond
			&& !check_set(vars->env, cmd[i], key, equal) && !check_set(vars->set, cmd[i], key, equal))
			creat_env_var(&vars->set, cmd[i], key, equal);
		free(key);
	}
	return (cond);
}

void pipes(t_vars *vars, t_cmds **cmds, int count)
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

int	redirection_blya(t_vars *vars, t_cmds **cmds, int i)		//naming
{
	if ((*cmds)[i].in_stat == 1 && err_mes(dup2(open((*cmds)[i].red_in,		//mehhhhhh
		O_RDONLY), 0) == -1, vars, NULL, CD_ERR))
			return (0);
	// else if ((*cmds)[i].in_stat == 2)
	// 	;
	if ((*cmds)[i].out_stat == 1 && err_mes(dup2(open((*cmds)[i].red_out,
		O_CREAT | O_TRUNC | O_WRONLY, 0644), 1) == -1, vars, NULL, CD_ERR))
			return (0);
	else if ((*cmds)[i].out_stat == 2 && err_mes(dup2(open((*cmds)[i].red_out,
		O_CREAT | O_APPEND | O_WRONLY, 0644), 1) == -1, vars, NULL, CD_ERR))
			return (0);
	return (1);
}

int redirect_in_out(t_vars *vars, t_cmds **cmds, int count, int i)
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
	return (redirection_blya(vars, cmds, i));
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

int	env_len(t_env *env)
{
	int		len;
	t_env	*head;

	len = 0;
	head = env;
	while (env)
	{
		len++;
		env = env->next;
	}
	env = head;
	return (len);
}

void	env_to_str(t_vars *vars)
{
	int		len;
	int		i;
	t_env	*head;

	len = env_len(vars->env);
	vars->env_var = malloc(sizeof(char *) * (len + 1));
	i = -1;
	head = vars->env;
	while (++i < len)
	{
		vars->env_var[i] = vars->env->line;
		vars->env = vars->env->next;
	}
	vars->env_var[i] = NULL;
	vars->env = head;
}

void	creating_exec_path(t_vars *vars)
{
	int		i;
	char	*tmp;

	env_to_str(vars);
	i = -1;
	while (vars->env_var[++i])
		if (!ft_strncmp(vars->env_var[i], "PATH=", 5))
			break ;
	if (vars->env_var[i])
	{
		tmp = ft_substr(vars->env_var[i], 5, ft_strlen(vars->env_var[i]) - 5);
		vars->paths = ft_split(tmp, ':');
		free(tmp);
	}
}

void	path_check(t_vars *vars, t_cmds **cmds, char *cmd, int i)
{
	char	*tmp;
	int		j;

	creating_exec_path(vars);
	if (access(cmd, X_OK) != -1)
	{
		(*cmds)[i].ex_cmd = ft_strdup(cmd);
		return ;
	}
	j = 0;
	while (vars->paths && vars->paths[j])
	{
		tmp = ft_strjoin(vars->paths[j], "/");
		(*cmds)[i].ex_cmd = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access((*cmds)[i].ex_cmd, X_OK) != -1)
			return ;
		j++;
		free((*cmds)[i].ex_cmd);
	}
	perror(cmd);
	exit(1);
}

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
				(*cmds)[i].red_out = ft_strdup(sp_split[j + 1]);		//error
				malloc_err(!(*cmds)[i].red_out, "creating redirection vars");
			}
			else if (!ft_strcmp(sp_split[j], "<") || !ft_strcmp(sp_split[j], "<<"))
			{
				if (!ft_strcmp(sp_split[j], "<"))
					(*cmds)[i].in_stat = 1;
				else
					(*cmds)[i].in_stat = 2;
				(*cmds)[i].red_in = ft_strdup(sp_split[j + 1]);		//error
				malloc_err(!(*cmds)[i].red_in, "creating redirection vars");
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

int read_input(t_vars *vars, t_cmds **cmds)
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

int main(int argc, char **argv, char **env)
{
	t_vars	vars;
	t_cmds	*cmds = NULL;
	int		i;

	vars.set = NULL;
	(void)argv;
	if (argc != 1)
	{
		perror("No such file or directory");
		exit(0);
	}
	vars.exit_stat = 0;
	vars.env = creat_env_list(env);
	while (1)
	{
		int count = read_input(&vars, &cmds);
		if (count == -1)
			continue;
		else if (err_mes(!count, &vars, NULL, PIPE_ERR))
			continue ;

		vars.fd_in = dup(0);
		vars.fd_out = dup(1);

		if (count == 1 && !redirect_in_out(&vars, &cmds, count, 0))
		{
			dup2(vars.fd_out, 1);
			continue ;		//leaks
		}

		if (!(count == 1 && check_builtins(&vars, cmds[0].cmd)) && !check_equal(&vars, cmds[0].cmd))
		{
			pipes(&vars, &cmds, count);
			i = -1;
			while (++i < count)
			{
				cmds[i].pid = fork();
				stop_program(cmds[i].pid == -1, NULL, "Fork error", vars.exit_stat);
				if (cmds[i].pid == 0)
				{
					if (!redirect_in_out(&vars, &cmds, count, i))
						exit(1);
					tolower_str(*cmds[i].cmd);
					path_check(&vars, &cmds, cmds[i].cmd[0], i);
					if (!check_builtins(&vars, cmds[i].cmd))
						exit(execve(cmds[i].ex_cmd, cmds[i].cmd, vars.env_var));
					exit(vars.exit_stat);
				}
			}
			close_pipes(&cmds, count);
			i = -1;
			while (++i < count)
				waitpid(cmds[i].pid, 0, 0);
		}

		dup2(vars.fd_in, 0);
		dup2(vars.fd_out, 1);

		i = -1;
		while (++i < count)
		{
			split_free(cmds[i].cmd);
			free(cmds[i].red_in);
			free(cmds[i].red_out);
			if (i != count - 1)
				free(cmds[i].pipe);
		}
		free(cmds);
	}
	return (0);
}
