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

typedef struct s_vars
{
	int		exit_stat;
	t_env	*env;
	t_env	*env_vars;
	char	**cmd;
	char	main_c;
	int		q_count;
}	t_vars;

void		pwd(t_vars *vars);
void		cd(t_vars *vars);
void		echo(t_vars *vars);
void		env(t_vars *vars, int cmd);
void		export(t_vars *vars);
void		unset(t_vars *vars);
void		exit_prog(t_vars *vars);
void		stop_program(int condition, char *cmd, char *issue, int exit_stat);
void		malloc_err(int condition, char *cmd);
void		err_mes(int condition, t_vars *vars, char *line, char *issue);
long long	ft_atoll(char *str);
void		split_free(char **split);
char		*tolower_str(char *str);
int			ft_isalnum_str(char *str, char c);
void		creat_env_var(t_env **env, char *cmd, char *key, long long equal);
int			check_env_vars(t_env *env, char *cmd, char *key, long long equal);
char		*rm_quotes(t_vars *vars, char *input_str);
t_env		*find_same_key(t_vars vars, char *input_str);
void		quotes_handler(t_vars *vars, char **input_str);
int			quotes_counter(char *input_str, char *main_c);
void		wait_quote(char **input_str, char c, int *count);

#endif
