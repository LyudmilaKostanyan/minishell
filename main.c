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

void check_equal(t_vars *vars, char **cmd)
{
	int i;
	long long equal;
	char *key;

	i = -1;
	while (cmd[++i])
	{
		equal = ft_strchr(cmd[i], '=') - cmd[i];
		key = ft_substr(cmd[i], 0, equal);
		malloc_err(!key, cmd[0]);
		if (equal >= 0 && !ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e') && !check_set(vars->env, cmd[i], key, equal) && !check_set(vars->set, cmd[i], key, equal))
			creat_env_var(&vars->set, cmd[i], key, equal);
		free(key);
	}
}

// int str_changes(t_vars *vars, t_cmds **cmds, char **pipe_splt, int count)
// {

// 	return (0);
// }

void pipes(t_vars *vars, t_cmds **cmds, int count)
{
	int i;

	i = -1;
	while (++i < count - 1)
	{
		(*cmds)[i].pipe = malloc(sizeof(int) * 2);
		malloc_err(!(*cmds)[i].pipe, "creat pipes");
	}
	i = -1;
	while (++i < count - 1)
	{
		if (pipe((*cmds)[i].pipe) == -1)
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

void redirect_in_out(int i, int count, t_cmds **cmds, t_vars *vars)
{
	vars->in_fd = dup(1);		//???????
	if (i == 0)
	{
		close((*cmds)[i].pipe[0]);
		dup2((*cmds)[i].pipe[1], 1);
	}
	else if (i == count - 1)
	{
		dup2((*cmds)[i - 1].pipe[0], 0);
		close((*cmds)[i - 1].pipe[1]);
	}
	else
	{
		dup2((*cmds)[i - 1].pipe[0], 0);
		dup2((*cmds)[i].pipe[1], 1);
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

void	path(t_vars *vars)
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

	path(vars);
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

int main(int argc, char **argv, char **env)
{
	t_vars	vars;
	t_cmds	*cmds;
	char	*input_str;
	char	**pipe_splt;
	int		i;

	input_str = NULL;
	pipe_splt = NULL;
	cmds = NULL;
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
		free(input_str);
		input_str = readline("\e[34mminishell$ \e[0m");
		stop_program(!input_str, NULL, "exit", vars.exit_stat);
		if (!*input_str)
			continue;
		split_free(pipe_splt);

		quotes_handler(&vars, &input_str);
		char *for_split;
		for_split = rm_quotes(&vars, input_str);
		if (!for_split)
			pipe_splt = ft_split(input_str, '|');
		else
			pipe_splt = ft_split(for_split, '|');
		free(for_split);
		malloc_err(!pipe_splt, "split cmds");
		if (err_mes(!*pipe_splt, &vars, NULL, PIPE_ERR))
			continue;
		int count = split_size(pipe_splt);
		free(cmds);
		cmds = malloc((sizeof(t_cmds) * count) + 1);
		malloc_err(!cmds, "creat cmds");
		i = -1;
		while (++i < count)
		{
			cmds[i].cmd = ft_split(pipe_splt[i], ' ');
			malloc_err(!cmds[i].cmd, "creating cmd list");
			// if (cmds[i])				!!!!
			int j = -1;
			while (cmds[i].cmd[++j])
				restore_spaces(&cmds[i].cmd[j]);
		}
		restore_spaces(&input_str);
		add_history(input_str);

		pipes(&vars, &cmds, count);
		i = -1;
		while (++i < count && count > 1)
		{
			cmds[i].pid = fork();
			stop_program(cmds[i].pid == -1, NULL, "Fork error", vars.exit_stat);
			if (cmds[i].pid == 0)
			{
				redirect_in_out(i, count, &cmds, &vars);
				check_equal(&vars, cmds[i].cmd);
				close_pipes(&cmds, count);
				path_check(&vars, &cmds, cmds[i].cmd[0], i);
				exit(execve(cmds[i].ex_cmd, cmds[i].cmd, vars.env_var));
			}
			close_pipes(&cmds, count);
		}
		i = -1;
		while (++i < count)
			waitpid(cmds[i].pid, 0, 0);
		i = -1;
		while (++i < count && count < 2)
		{
			check_equal(&vars, cmds[i].cmd);
			tolower_str(*cmds[i].cmd);
			check_builtins(&vars, cmds[i].cmd);
		}
		i = -1;
		while (++i < count)
		{
			split_free(cmds[i].cmd);
			if (i != count - 1)
				free(cmds[i].pipe);
		}
	}
	return (0);
}
