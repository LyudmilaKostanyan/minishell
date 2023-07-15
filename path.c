#include "minishell.h"

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
