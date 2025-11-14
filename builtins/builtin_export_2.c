/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:54:06 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:03:37 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

static int	handle_with_equal_sign(char *cmd, t_env **env, t_garbage **garb)
{
	char	*type;
	char	*content;

	type = malloc(env_len(cmd, 0));
	if (!type)
		return (1);
	if (allocate_content(&content, cmd, type))
		return (1);
	g_collecter_env(garb, type);
	g_collecter_env(garb, content);
	env_cpy(type, content, cmd);
	env_remove(type, env);
	env_add(type, content, env, garb);
	return (0);
}

static int	process_single_export(char *cmd, t_env **env, t_garbage **garb)
{
	char	*equal;

	if (!is_valid_export(cmd))
		return (handle_invalid_export(cmd));
	equal = ft_strchr(cmd, '=');
	if (!equal)
		return (handle_no_equal_sign(cmd, env, garb));
	else
		return (handle_with_equal_sign(cmd, env, garb));
}

static int	process_export_args(char **cmd, t_env **env, t_garbage **garb)
{
	int	i;
	int	ret;
	int	current_ret;

	i = 1;
	ret = 0;
	while (cmd[i])
	{
		current_ret = process_single_export(cmd[i], env, garb);
		if (current_ret == 1)
			ret = 1;
		else if (current_ret > 1)
			return (current_ret);
		i++;
	}
	return (ret);
}

int	builtin_export(char **cmd, t_env **env, t_garbage **garb)
{
	if (!cmd || !env || !(*env)->type)
		return (1);
	if (!cmd[1])
		return (export_out(*env, garb));
	return (process_export_args(cmd, env, garb));
}
