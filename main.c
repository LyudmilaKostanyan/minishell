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

t_env	*creat_env_list(char **env)
{
	t_env	*head;
	t_env	*node;
	int		equal;

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

int	check_builtins(char *cmd, t_vars *vars)
{
	if (!ft_strcmp(cmd, "pwd"))
		pwd(vars);
	else if (!ft_strcmp(cmd, "cd"))
		cd(vars);
	else if (!ft_strcmp(cmd, "env"))
		env(vars, 1);
	else if (!ft_strcmp(cmd, "echo"))
		echo(vars);
	else if (!ft_strcmp(cmd, "export"))
		export(vars);
	else if (!ft_strcmp(cmd, "unset"))
		unset(vars);
	else if (!ft_strcmp(cmd, "exit"))
		exit_prog(vars);
	else
		return (0);
	return (1);
}

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
			count--;
			while ((*input_str)[++i] && count)
			{
				if ((*input_str)[i] == vars->main_c)
					count--;
				if ((*input_str)[i] == 32)
					(*input_str)[i] = 1;
			}
			if (!(*input_str)[i])
				break ;							//annormal
		}
	}
}

t_env	*find_var_value(t_env *env, char *input_str)
{
	char		*key;
	long long	space_pos;

	while (*input_str)
	{
		if (*input_str == '$')
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
					return (env);
				env = env->next;
			}
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
			if (*tmp == '$')
			{
				int j = -1;
				while (env->value[++j])
					out_str[++i] = env->value[j];
				tmp++;
				j = -1;
				while (*tmp && *tmp == env->key[++j])
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

void	restore_spaces(char **str)
{
	int	i;

	i = -1;
	while ((*str)[++i])
		if ((*str)[i] == 1)
			(*str)[i] = 32;
}

void	check_equal(t_vars	*vars)
{
	int			i;
	long long	equal;
	char		*key;

	i = -1;
	while (vars->cmd[++i])
	{
		equal = (uintptr_t)ft_strchr(vars->cmd[i], '=')
			- (uintptr_t)vars->cmd[i];
		key = ft_substr(vars->cmd[i], 0, equal);
		malloc_err(!key, vars->cmd[0]);
		if (!ft_isdigit(*key) && *key && ft_isalnum_str(key, 'e') && equal >= 0
			&& !check_env_vars(vars->env_vars, vars->cmd[i], key, equal))
			creat_env_var(&vars->env_vars, vars->cmd[i], key, equal);//mall_err!!!
		free(key);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_vars	vars;
	char	*input_str;
	char	*for_split;

	(void)argv;
	if (argc != 1)
	{
		perror("No such file or directory");
		exit(0);
	}
	vars.exit_stat = 0;
	vars.env = creat_env_list(env);
	while (1)
	{
		input_str = readline("\e[34mminishell$ \e[0m");
		if (!input_str)
		{
			printf("exit\n"); // ^D ??????????
			exit(vars.exit_stat);
		}
		else if (!*input_str)
			continue ;
		quotes_handler(&vars, &input_str);
		for_split = rm_quotes(&vars, input_str);
		if (!for_split)
			vars.cmd = ft_split(input_str, ' ');
		else
			vars.cmd = ft_split(for_split, ' ');
		malloc_err(!vars.cmd, "creating cmd list");
		int	i = -1;
		while (vars.cmd[++i])
			restore_spaces(&vars.cmd[i]);
		restore_spaces(&input_str);
		add_history(input_str);
		free(input_str);
		if (!*vars.cmd)
			continue ;
		if (!check_builtins(tolower_str(&*vars.cmd), &vars))
			check_equal(&vars);
		split_free(vars.cmd);
	}
	return (0);
}
