#include "minishell.h"

void	wait_stop(char **input_str, char c, int *count)
{
	char	*line;
	char	*tmp;

	while (1)
	{
		tmp = ft_strjoin(*input_str, "\n");
		malloc_err(!tmp, "reading input");
		free(*input_str);
		*input_str = tmp;
		line = readline("> ");
		if (!line)
		{
			printf("bash: unexpected EOF while looking for matching `\"\'\nbash: syntax error: unexpected end of file\n"); // ^D ??????????
			break ;
		}
		tmp = ft_strjoin(*input_str, line);
		malloc_err(!tmp, "reading input");
		free(*input_str);
		*input_str = tmp;
		tmp = ft_strchr(line, c);
		if (tmp)
		{
			(*count)++;
			if (!ft_strchr(tmp + 1, c))
			{
				break ;
				free(line);
			}
		}
		free(line);
	}
}

int	quotes_counter(char *input_str, char *main_c)
{
	int		count;

	count = 0;
	while (*input_str)
	{
		if (*input_str == '\"' || *input_str == '\'')
		{
			*main_c = *input_str;
			break ;
		}
		input_str++;
	}
	while (*input_str)
	{
		if (*input_str == *main_c)
			count++;
		input_str++;
	}
	return (count);
}

void	quotes_handler(t_vars *vars, char **input_str)
{
	int		i;
	int		count;

	vars->q_count = quotes_counter(*input_str, &vars->main_c);
	if (!vars->q_count)
		return ;
	if (vars->q_count % 2 != 0)
		wait_stop(input_str, vars->main_c, &vars->q_count);
	count = vars->q_count;
	i = -1;
	while ((*input_str)[++i])
	{
		if ((*input_str)[i] == vars->main_c)
		{
			while ((*input_str)[i] && (*input_str)[i + 1] && count)
			{
				if ((*input_str)[i] == vars->main_c)
					count--;
				if ((*input_str)[i] == 32)
					(*input_str)[i] = 1;
				i++;
			}
		}
	}
}

t_env	*find_var_value(t_env *env, char *input_str)
{
	char		*key;
	long long	space_pos;

	while (*input_str)
	{
		if (*input_str == '$' && *(input_str + 1))
		{
			input_str++;
			if (ft_strchr(input_str, 1))
				space_pos = (uintptr_t)ft_strchr(input_str, 1) - (uintptr_t)input_str;
			else if (ft_strchr(input_str, 32))
				space_pos = (uintptr_t)ft_strchr(input_str, 32) - (uintptr_t)input_str;
			else
				space_pos = ft_strlen(input_str);
			key = ft_substr(input_str, 0, space_pos);
			malloc_err(!key, "check $ case");
			while (env)
			{
				if (!ft_strcmp(env->key, key))
				{
					free(key);
					return (env);
				}
				env = env->next;
			}
			free(key);
		}
		input_str++;
	}
	return (NULL);
}

char	*rm_quotes(t_vars *vars, char *input_str) //$!!!!!!!!!!!! //""
{
	char	*out_str;
	int		i;
	char	*tmp;
	int		val_len;
	int		key_len;
	t_env	*env;

	val_len = 0;
	key_len = 0;
	env = find_var_value(vars->env_vars, input_str);
	if (env)
	{
		key_len = ft_strlen(env->key) + 1;
		val_len = ft_strlen(env->value);
	}
	else if (!vars->main_c)
		return (NULL);
	out_str = malloc(ft_strlen(input_str) + val_len - key_len - vars->q_count + 1);	//-0!!!!!!!!
	malloc_err(!out_str, "rm_quotes");
	i = -1;
	tmp = input_str;
	while (*tmp)
	{
		if (*tmp != vars->main_c)
		{
			if (*tmp == '$' && env)
			{
				int j = -1;
				while (env->value[++j])
					out_str[++i] = env->value[j];
				tmp++;
				j = -1;
				while (*(tmp + 1) && *tmp == env->key[++j])
					tmp++;
			}
			else
				out_str[++i] = *tmp;
		}
		tmp++;
	}
	out_str[++i] = 0;
	return (out_str);
}
