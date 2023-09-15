#include "minishell.h"

int	free_node(t_env **list, t_env *env, char *cmd)
{
	t_env	*tmp;

	if (!ft_strcmp(env->key, cmd))
	{
		*list = env->next;
		free(env->line);
		free(env->key);
		free(env->value);
		free(env);
	}
	else if (env->next && !ft_strcmp(env->next->key, cmd))
	{
		tmp = env->next->next;
		free(env->next->line);
		free(env->next->key);
		free(env->next->value);
		free(env->next);
		env->next = tmp;
	}
	else
		return (0);
	return (1);
}

void	find_node(t_vars *vars, char *cmd)
{
	t_env	*env;

	env = vars->env;
	while (env)
	{
		if (free_node(&vars->env, env, cmd))
			break ;
		env = env->next;
	}
	env = vars->set;
	while (env)
	{
		if (free_node(&vars->set, env, cmd))
			break ;
		env = env->next;
	}
}
