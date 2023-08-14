#include "minishell.h"


void	malloc_err(int condition, char *cmd)
{
	if (condition)
	{
		printf("minishell: malloc error: %s\n", cmd);
		exit(1);
	}
}

void	stop_program(int condition, char *cmd, char *issue)
{
	if (condition)
	{
		if (!cmd && !ft_strcmp(issue, "exit"))
			printf("%s\n", issue);
		else if (!cmd)
			printf("minishell: %s\n", issue);
		else
			printf("minishell: %s: %s\n", cmd, issue);
		if (!ft_strcmp(cmd, "exit"))
			exit(255);
		exit(g_exit_status);
	}
}

int	err_mes(int condition, char *cmd, char *line, char *issue)
{
	if (condition)
	{
		if (!cmd && !line)
			printf("minishell: %s\n", issue);
		else if (!line)
			printf("minishell: %s: %s\n", cmd, issue);
		else
			printf("minishell: %s: `%s': %s\n", cmd, line, issue);
		// if (!cmd)
		// 	printf("minishell: %s\n", line);
		// else if (*cmd == 'u' || *cmd == 'e')
		// 	printf("minishell: %s: `%s': %s\n", cmd, line, E_U_ERR);
		// else if (*cmd[0] == 'c')
		// {
		// 	if (!cmd[2])
		// 		printf("minishell: %s: `%s': %s\n", cmd, line, CD_ERR);
		// 	else
		// 		printf("minishell: %s: %s\n", cmd, TMA);
		// }
		g_exit_status = 1;
		return (1);
	}
	return (0);
}
