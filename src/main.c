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

int	g_exit_status = 0;

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
		pwd();
	else if (!ft_strcmp(*cmd, "cd"))
		cd(vars, cmd);
	else if (!ft_strcmp(*cmd, "env"))
		env(vars, 1);
	else if (!ft_strcmp(*cmd, "echo"))
		echo(cmd);
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

void	free_cmds(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	dup2(vars->fd_in, 0);
	dup2(vars->fd_out, 1);
	i = -1;
	while (++i < count)
	{
		split_free((*cmds)[i].cmd);
		free((*cmds)[i].red_in);
		free((*cmds)[i].red_out);
		if (i != count - 1)
			free((*cmds)[i].pipe);
	}
	free(*cmds);
}

void	processes(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	pipes(cmds, count);
	i = -1;
	while (++i < count)
	{
		(*cmds)[i].pid = fork();
		stop_program((*cmds)[i].pid == -1, NULL, "Fork error");
		if ((*cmds)[i].pid == 0)
		{
			if (!redirect_pipes(vars, cmds, count, i))
				exit(1);
			tolower_str(*(*cmds)[i].cmd);
			path_check(vars, cmds, (*cmds)[i].cmd[0], i);
			if (*(*cmds)[i].cmd && !check_builtins(vars, (*cmds)[i].cmd))
				exit(execve((*cmds)[i].ex_cmd, (*cmds)[i].cmd, vars->env_var));
			exit(g_exit_status);
		}
	}
	close_pipes(cmds, count);
	i = -1;
	while (++i < count)
	{
		waitpid((*cmds)[i].pid, &g_exit_status, 0);
        if (!WIFSIGNALED(g_exit_status))
		    g_exit_status = WEXITSTATUS(g_exit_status);
        else
            g_exit_status += 128;
	}
}

void	action(int signal)
{
	if (signal == SIGINT)
		g_exit_status = 1;
	rl_replace_line("", 0);
	rl_done = 1;
}

int	empty()
{
	return 0;
}

int main(int argc, char **argv, char **env)
{
	t_vars				vars;
	t_cmds				*cmds;
	struct sigaction	sig;

	rl_event_hook = &empty;
	rl_catch_signals = 0;
	sigemptyset(&sig.sa_mask);
	sig.sa_handler = &action;
	sig.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sig, NULL);
	sig.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sig, NULL);
	cmds = NULL;
	vars.set = NULL;
	(void)argv;
	if (argc != 1)
	{
		perror("No such file or directory");
		exit(0);
	}
	g_exit_status = 0;
	vars.env = creat_env_list(env);
	while (1)
	{
		vars.fd_in = dup(0);
		vars.fd_out = dup(1);
		int count = read_input(&vars, &cmds);
		if (count == -1 || err_mes(!count, NULL, NULL, PIPE_ERR)
			|| err_mes(count == -2, "fork", NULL, TMP))	//second err_mes
			continue;
		if (count == 1 && (!ft_strcmp(*cmds[0].cmd, "pwd") || !ft_strcmp(*cmds[0].cmd, "cd")	//!!!!!!!!
			|| !ft_strcmp(*cmds[0].cmd, "echo") || !ft_strcmp(*cmds[0].cmd, "export")
			|| !ft_strcmp(*cmds[0].cmd, "unset") || !ft_strcmp(*cmds[0].cmd, "exit")
			|| !ft_strcmp(*cmds[0].cmd, "env")) && !redirect_pipes(&vars, &cmds, count, 0))
		{
			free_cmds(&vars, &cmds, count);
			continue ;
		}
		if (!(count == 1 && check_builtins(&vars, cmds[0].cmd)) && !check_equal(&vars, cmds[0].cmd))
			processes(&vars, &cmds, count);
		free_cmds(&vars, &cmds, count);
	}
	return (0);
}
