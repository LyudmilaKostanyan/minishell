/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:18:14 by lykostan          #+#    #+#             */
/*   Updated: 2023/09/15 18:50:31 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define EXIT_ERR "numeric argument required"
# define QUOTES_ERR "input correct number of quotes"
# define CD_ERR "No such file or directory"
# define E_U_ERR "not a valid identifier"
# define PIPE_ERR "syntax error near unexpected token `|'"
# define TMA "too many arguments"
# define IO "redirection input/output"
# define TMP "Resource temporarily unavailable"
# define CRV "creating redirection vars"
# define CCL "creating cmd list"
# define PD "Permission denied"

# include <limits.h>
# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include "readline/readline.h"
# include "readline/history.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/ioctl.h>
# include <libft.h>

extern int	g_exit_status;

typedef struct s_env
{
	char			*line;
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_cmds
{
	char			**cmd;
	char			*ex_cmd;
	char			**path;
	pid_t			pid;
	int				*pipe;
	char			*red_in;
	char			*red_out;
	int				in_stat;
	int				out_stat;
}	t_cmds;

typedef struct s_vars
{
	char				*out_str;
	int					exit_stat;
	t_env				*env;
	char				**true_env;
	char				**env_var;
	t_env				*set;
	char				main_c;
	int					q_count;
	int					fd_in;
	int					fd_out;
	char				**paths;
	int					hd_stat;
	long long			equal;
	char				*key;
	struct sigaction	sig;
}	t_vars;

typedef struct s_mall_size
{
	int	sp_count;
	int	key_len;
	int	val_len;
}	t_mall_size;

void		pwd(t_vars *vars);
void		cd(t_vars *vars, char **cmd);
void		echo(char **cmd);
void		env(t_vars *vars, int cmd);
void		export(t_vars *vars, char **cmd);
void		unset(t_vars *vars, char **cmd);
void		exit_prog(t_vars *vars, char **cmd);
void		malloc_err(int condition, char *cmd, t_vars *vars);
void		split_free(char **split);
void		creat_env_var(t_vars *vars, t_env **env, char *cmd, char *key);
void		fill_out_str(char **tmp, char **out_str, t_env *env, int *i);
void		find_main_c(t_vars *vars, char *tmp);
void		restore_spaces(char **str);
void		env_to_str(t_vars *vars);
void		creating_exec_path(t_vars *vars);
void		pipes(t_vars *vars, t_cmds **cmds, int count);
void		close_pipes(t_cmds **cmds, int count);
void		stop_program(int condition, char *cmd, char *issue, t_vars *vars);
void		add_remove_shlvl(char **env, int i);
void		find_node(t_vars *vars, char *cmd);
void		merge_key_value(t_vars *vars, t_env *node);
void		free_cmds(t_vars *vars, t_cmds **cmds, int count);
void		action(int signal);

char		*tolower_str(char *str);
char		*rm_quotes(t_vars *vars, char *input_str);

t_env		*key_cmp(t_vars vars, char **input_str);
t_env		*find_key(t_vars vars, char *key);
t_env		*find_same_key(t_vars vars, char *input_str);
t_env		*checking_env_key(t_env *env, char *key);
t_env		*creat_env_list(t_vars *vars, char **env);

long long	ft_atoll(t_vars *vars, char *str);
int			empty(void);
int			ft_isalnum_str(char *str, char c);
int			check_set(t_vars *vars, t_env *env, char *cmd, char *key);
int			wait_quote(t_vars *vars, char **input_str, char c, int *count);
int			split_size(char **split);
int			read_input(t_vars *vars, t_cmds **cmds);
int			env_len(t_env *env);
int			path_check(t_vars *vars, t_cmds **cmds, char *cmd, int i);
int			redirection(t_vars *vars, t_cmds **cmds, int i);
int			redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i);
int			here_doc(t_vars *vars, char *end);
int			err_mes(int condition, char *cmd, char *line, char *issue);
int			add_value(t_vars *vars, char *cmd, t_env **env);
int			count_key_val(t_vars vars, char *input_str,
				t_mall_size *mall_size, int i);
int			merge_cmds(t_vars *vars, t_cmds **cmds,
				char **pipe_splt, int count);
int			check_redirection(t_cmds *cmds, int count);
int			check_builtins(t_vars *vars, char **cmd);
int			check_equal(t_vars *vars, char **cmd);
int			quotes_handler(t_vars *vars, char **input_str);
void		sig_handler(int sig);

#endif
