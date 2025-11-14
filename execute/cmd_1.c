/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:31:31 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:46:27 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../ft_printf/ft_printf.h"
#include "../libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

char	*find_command_path(char *cmd, t_garbage **garb, t_env *env)
{
	char	*path;
	char	*norm;
	char	**dirs;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (g_collecter(garb, ft_strdup(cmd), 1));
		perror(cmd);
		return (NULL);
	}
	path = get_env_path(list_to_arr(env, garb));
	if (!path)
		return (NULL);
	norm = normalize_path(path, garb);
	if (!norm)
		return (NULL);
	dirs = split_dirs(norm, garb);
	if (!dirs)
		return (NULL);
	return (search_in_dirs(dirs, cmd, garb));
}

int	ret_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	check_execution_permissions(const char *path, char *cmd,
		char *cmd_path)
{
	if (access(path, X_OK) == -1)
	{
		ft_printf("minishell: %s: Permission denied\n", path);
		return (126);
	}
	if (!cmd_path)
	{
		if (ft_strncmp(cmd, "./", 1) != 0)
			ft_printf("minishell: %s: command not found\n", cmd);
		return (127);
	}
	return (0);
}

int	validate_command_path(const char *path, char *cmd, char *cmd_path)
{
	struct stat	st;
	int			result;

	if (!ft_strchr(cmd, '/') && !cmd_path)
	{
		ft_printf("%s: command not found\n", cmd);
		return (127);
	}
	if (stat(path, &st) == -1)
	{
		ft_printf("minishell: %s: No such file or directory\n", path);
		return (127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_printf("minishell: %s: Is a directory\n", path);
		return (126);
	}
	result = check_execution_permissions(path, cmd, cmd_path);
	if (result != 0)
		return (result);
	return (0);
}
