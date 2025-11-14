/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:02:39 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 01:20:52 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <unistd.h>

void	signal_handler(int sig, siginfo_t *info, void *context)
{
	(void)sig;
	(void)info;
	(void)context;
}

int	is_punctuation(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, ".") == 0 || ft_strcmp(str, "") == 0 || ft_strcmp(str,
			"./") == 0 || ft_strcmp(str, "/") == 0)
		return (1);
	return (0);
}

int	exec_builtin(char **cmd, t_env *env, t_garbage **garb)
{
	if (ft_strcmp(cmd[0], "echo") == 0)
		return (builtin_echo(cmd));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (builtin_cd(cmd, &env, garb));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (builtin_pwd(garb, env));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (builtin_export(cmd, &env, garb));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (builtin_unset(cmd, &env));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (builtin_env(cmd, env));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (builtin_exit(cmd));
	else if (is_punctuation(cmd[0]))
		return (builtin_punctuation(cmd));
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0 || is_punctuation(cmd))
		return (1);
	return (0);
}

char	*get_env_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}
