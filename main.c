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

int str_changes(t_vars *vars, t_cmds **cmds, char **pipe_splt, int count)
{
	char *for_split;
	int j;

	int i = -1;
	while (++i < count)
	{
		quotes_handler(vars, &pipe_splt[i]);
		for_split = rm_quotes(vars, pipe_splt[i]);
		if (!for_split)
			(*cmds)[i].cmd = ft_split(pipe_splt[i], ' ');
		else
			(*cmds)[i].cmd = ft_split(for_split, ' ');
		free(for_split);
		if (!*(*cmds)[i].cmd && count > 1)
			return (1);
		else if (!*(*cmds)[i].cmd)
			return (-1);
		malloc_err(!(*cmds)[i].cmd, "creating cmd list"); //???
		j = -1;
		while ((*cmds)[i].cmd[++j])
			restore_spaces(&(*cmds)[i].cmd[j]);
		restore_spaces(&pipe_splt[i]);
		add_history(pipe_splt[i]);		//out while
	}
	return (0);
}

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
	vars->env_var = malloc((sizeof(char *) * len) + 1);
	i = -1;
	head = vars->env;
	while (++i < len)
	{
		vars->env_var[i] = vars->env->line;
		vars->env = vars->env->next;
	}
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
	t_vars vars;
	char *input_str;
	t_cmds *cmds;

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
		// system("leaks minishell");
		input_str = readline("\e[34mminishell$ \e[0m");
		stop_program(!input_str, NULL, "exit", vars.exit_stat);
		if (!*input_str)
			continue;
		char **pipe_splt = ft_split(input_str, '|');
		free(input_str);
		malloc_err(!pipe_splt, "split cmds");
		if (err_mes(!*pipe_splt, &vars, NULL, PIPE_ERR))
			continue;
		int count = split_size(pipe_splt);
		cmds = malloc((sizeof(t_cmds) * count) + 1);
		malloc_err(!cmds, "creat cmds");
		int i;
		i = str_changes(&vars, &cmds, pipe_splt, count);
		if (err_mes(i, &vars, NULL, PIPE_ERR))
		{
			// i = -1;
			// while (++i < count)						//payman
			// 	split_free(cmds[i].cmd);
			split_free(pipe_splt);
			free(cmds);
			continue;
		}
		else if (i == -1)
		{
			i = -1;
			while (++i < count)
				split_free(cmds[i].cmd);
			split_free(pipe_splt);
			free(cmds);
			continue;
		}
		// system("leaks minishell");
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
				tolower_str(*cmds[i].cmd);
				if (check_builtins(&vars, cmds[i].cmd))
					exit(vars.exit_stat);
				else
				{
					path_check(&vars, &cmds, cmds[i].cmd[0], i);
					printf("%s\n", *cmds[i].cmd);
					exit(execve(cmds[i].ex_cmd, cmds[i].cmd, vars.env_var));
				}
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
		split_free(pipe_splt);
		free(cmds);
	}
	return (0);
}
