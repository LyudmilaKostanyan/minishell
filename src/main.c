/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:17:51 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/12 19:42:00 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_status = 0;

static void	processes_help(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	i = -1;
	while (++i < count)
	{
		(*cmds)[i].pid = fork();
		stop_program((*cmds)[i].pid == -1, NULL, "Fork error", vars);
		if ((*cmds)[i].pid == 0)
		{
			if (!redirect_pipes(vars, cmds, count, i))
				exit(1);
			tolower_str(*(*cmds)[i].cmd);
			close_pipes(cmds, count);
			vars->sig.sa_handler = SIG_DFL;
			sigaction(SIGQUIT, &vars->sig, NULL);
			if (*(*cmds)[i].cmd && !check_builtins(vars, (*cmds)[i].cmd)
				&& path_check(vars, cmds, *(*cmds)[i].cmd, i))
				exit(execve((*cmds)[i].ex_cmd, (*cmds)[i].cmd, vars->env_var));
			exit(g_exit_status);
		}
	}
}

void	processes(t_vars *vars, t_cmds **cmds, int count)
{
	int	i;

	pipes(vars, cmds, count);
	processes_help(vars, cmds, count);
	close_pipes(cmds, count);
	i = -1;
	while (++i < count)
	{
		waitpid((*cmds)[i].pid, &g_exit_status, 0);
		if (WTERMSIG(g_exit_status) == SIGQUIT)
			printf("Quit: 3\n");
		if (!WIFSIGNALED(g_exit_status))
			g_exit_status = WEXITSTATUS(g_exit_status);
		else
			g_exit_status += 128;
	}
}

static int	strcmp_help(const char *command)
{
	return ((!ft_strcmp(command, "pwd")
			|| !ft_strcmp(command, "cd") || !ft_strcmp(command, "echo")
			|| !ft_strcmp(command, "export") || !ft_strcmp(command, "unset")
			|| !ft_strcmp(command, "exit") || !ft_strcmp(command, "env")));
}

static void	main_help(t_vars vars, t_cmds *cmds, char **env)
{
	int	count;

	vars.env = creat_env_list(&vars, env);
	while (1)
	{
		vars.fd_in = dup(0);
		vars.fd_out = dup(1);
		count = read_input(&vars, &cmds);
		if (count == -1 || err_mes(!count, NULL, PIPE_ERR, &vars)
			|| !check_redirection(&vars, cmds, count))
		{
			free_cmds(&vars, &cmds, count);
			continue ;
		}
		if (count == 1 && strcmp_help(*cmds[0].cmd)
			&& !redirect_pipes(&vars, &cmds, count, 0))
		{
			free_cmds(&vars, &cmds, count);
			continue ;
		}
		if (!(count == 1 && check_builtins(&vars, cmds[0].cmd))
			&& !check_equal(&vars, cmds[0].cmd))
			processes(&vars, &cmds, count);
		free_cmds(&vars, &cmds, count);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_vars				vars;
	t_cmds				*cmds;

	rl_event_hook = &empty;
	rl_catch_signals = 0;
	sigemptyset(&vars.sig.sa_mask);
	vars.sig.sa_handler = &action;
	vars.sig.sa_flags = SA_RESTART;
	sigaction(SIGINT, &vars.sig, NULL);
	vars.sig.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &vars.sig, NULL);
	cmds = NULL;
	vars.set = NULL;
	(void)argv;
	vars.true_env = env;
	add_remove_shlvl(vars.true_env, 1);
	if (argc != 1)
	{
		perror("No such file or directory");
		add_remove_shlvl(vars.true_env, -42);
		exit(0);
	}
	g_exit_status = 0;
	main_help(vars, cmds, env);
}
