#include "minishell.h"

int	creat_out_str(t_vars *vars, char *input_str, char **out_str)
{
	t_mall_size	mall_size;
	int			cond;

	mall_size.sp_count = 0;
	mall_size.key_len = 0;
	mall_size.val_len = 0;
	mall_size.exit_stat_len = 0;
	cond = count_key_val(*vars, input_str, &mall_size, 1);
	if (!vars->main_c && !mall_size.key_len && !cond && !mall_size.sp_count)
		return (0);
	*out_str = malloc(ft_strlen(input_str) + mall_size.val_len
			- mall_size.key_len - vars->q_count
			+ mall_size.exit_stat_len + mall_size.sp_count + 1);
	malloc_err(!*out_str, "rm_quotes", vars);
	return (1);
}

void	fill_out_str(char **tmp, char **out_str, t_env *env, int *i)
{
	int	j;

	j = -1;
	while (env->value[++j])
		(*out_str)[++(*i)] = env->value[j];
	(*tmp)++;
	j = -1;
	while (*(*tmp + 1) && *(*tmp + 1) == env->key[++j + 1])
		(*tmp)++;
}
