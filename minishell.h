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

# define MALL_ERR "malloc error"
# define EXIT_ERR "numeric argument required"

typedef struct s_env
{
	char			*line;
	// char			*key;
	// char			*value;
	struct s_env	*next;
	// struct s_env	*prev;
}	t_env;

typedef struct s_vars
{
	int		exit_stat;
	t_env	*env;
	char	**cmd;
}	t_vars;

void		pwd(t_vars *vars);
void		cd(t_vars *vars);
void		echo(t_vars *vars);
void		env(t_vars *vars);
void		export(t_vars *vars);
void		unset(t_vars *vars);
void		exit_prog(t_vars *vars);
void		stop_program(int condition, char *cmd, char *issue, int exit_stat);
long long	ft_atoll(char *str);
void		split_free(char **split);
char		*tolower_str(char **str);
int			ft_isalnum_str(char *str);

#endif
