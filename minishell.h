/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lykostan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 19:18:14 by lykostan          #+#    #+#             */
/*   Updated: 2023/06/14 19:18:16 by lykostan         ###   ########.fr       */
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

# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <libft.h>

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

void		pwd(t_vars *vars);
void		cd(t_vars *vars, char **cmd);
void		echo(t_vars *vars, char **cmd);
void		env(t_vars *vars, int cmd);
void		export(t_vars *vars, char **cmd);
void		unset(t_vars *vars, char **cmd);
void		exit_prog(char **cmd);
void		stop_program(int condition, char *cmd, char *issue, int exit_stat);
void		malloc_err(int condition, char *cmd);
int			err_mes(int condition, t_vars *vars, char **cmd, char *line);
long long	ft_atoll(char *str);
void		split_free(char **split);
char		*tolower_str(char *str);
int			ft_isalnum_str(char *str, char c);
void		creat_env_var(t_env **env, char *cmd, char *key, long long equal);
int			check_set(t_env *env, char *cmd, char *key, long long equal);
t_env		*key_cmp(t_vars vars, char *key);
char		*rm_quotes(t_vars *vars, char *input_str);
void		find_main_c(t_vars *vars, char *tmp);
t_env		*find_same_key(t_vars vars, char *input_str);
void		quotes_handler(t_vars *vars, char **input_str);
void		wait_quote(char **input_str, char c, int *count);
int			split_size(char **split);

#endif
