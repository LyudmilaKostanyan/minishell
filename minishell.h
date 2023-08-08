/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tgalyaut <tgalyaut@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:18:14 by lykostan          #+#    #+#             */
/*   Updated: 2023/08/01 22:58:34 by tgalyaut         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define EXIT_ERR "numeric argument required"
# define QUOTES_ERR "bash: unexpected EOF while looking for matching `\"\'\n"
# define QUOTES_SYN_ERR "bash: syntax error: unexpected end of file\n"
# define CD_ERR "No such file or directory"
# define E_U_ERR "not a valid identifier"
# define PIPE_ERR "syntax error near unexpected token `|'"
# define TMA "too many arguments"
# define IO "redirection input/output"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

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
	char	**cmd;
	char	*ex_cmd;
	char	**path;
	pid_t	pid;
	int		*pipe;
	char	*red_in;
	char	*red_out;
	int		in_stat;
	int		out_stat;
}	t_cmds;

typedef struct s_vars
{
	int		exit_stat;
	t_env	*env;
	char	**env_var;
	t_env	*set;
	char	main_c;
	int		q_count;
	int		fd_in;
	int		fd_out;
	char	**paths;
}	t_vars;

typedef struct s_mall_size
{
	int	sp_count;
	int	key_len;
	int	val_len;
}	t_mall_size;

void		pwd(t_vars *vars);
void		cd(t_vars *vars, char **cmd);
void		echo(t_vars *vars, char **cmd);
void		env(t_vars *vars, int cmd);
void		export(t_vars *vars, char **cmd);
void		unset(t_vars *vars, char **cmd);
void		exit_prog(char **cmd);
void		stop_program(int condition, char *cmd, char *issue, int exit_stat);
void		malloc_err(int condition, char *cmd);
void		creat_env_var(t_env **env, char *cmd, char *key, long long equal);
void		fill_out_str(char **tmp, char **out_str, t_env *env, int *i);
void		split_free(char **split);
void		find_main_c(t_vars *vars, char *tmp);
void		quotes_handler(t_vars *vars, char **input_str);
void		wait_quote(char **input_str, char c, int *count);
void		restore_spaces(char **str);
void		merge_key_value(t_env *node);
void		env_to_str(t_vars *vars);
void		creating_exec_path(t_vars *vars);
void		path_check(t_vars *vars, t_cmds **cmds, char *cmd, int i);
void		pipes(t_vars *vars, t_cmds **cmds, int count);
void		close_pipes(t_cmds **cmds, int count);
void		malloc_err(int condition, char *cmd);
void		stop_program(int condition, char *cmd, char *issue, int exit_stat);

char		*tolower_str(char *str);
char		*rm_quotes(t_vars *vars, char *input_str);
char		*get_next_line(int fd);
char		*ft_strdup_gnl(char *s, int start, int j);
char		*ft_strjoin_gnl(char *s1, char *s2);

long long	ft_atoll(char *str);
int			err_mes(int condition, t_vars *vars, char **cmd, char *line);
int			ft_isalnum_str(char *str, char c);
int			check_set(t_env *env, char *cmd, char *key, long long equal);
int			split_size(char **split);
int			ft_strchr_gnl(char *str, int c);
int			read_input(t_vars *vars, t_cmds **cmds);
int			merge_cmds(t_cmds **cmds, char **pipe_splt, char **input_str);
int			env_len(t_env *env);
int			redirection(t_vars *vars, t_cmds **cmds, int i);
int			redirect_pipes(t_vars *vars, t_cmds **cmds, int count, int i);
int			err_mes(int condition, t_vars *vars, char **cmd, char *line);
// int			here_doc(t_vars *vars, char *end);

t_env		*find_key(t_vars vars, char *key);
t_env		*key_cmp(t_vars vars, char **input_str);
t_env		*find_same_key(t_vars vars, char *input_str);

#endif
