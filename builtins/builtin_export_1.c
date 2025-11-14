/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:54:37 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:09:53 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

int	get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->type, key, ft_strlen(key)) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}

int	is_valid_export(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (i == 0 && !(ft_isalpha(str[i]) || str[i] == '_'))
			return (0);
		else if (i > 0 && !(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	if (i == 0 && str[i] == '=')
		return (0);
	return (1);
}

int	handle_invalid_export(char *cmd)
{
	ft_putstr_fd("export: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (1);
}

int	handle_no_equal_sign(char *cmd, t_env **env, t_garbage **garb)
{
	char	*type;
	char	*content;

	if (get_env_value(*env, cmd))
		return (0);
	type = ft_strdup(cmd);
	if (!type)
		return (1);
	content = NULL;
	g_collecter_env(garb, type);
	g_collecter_env(garb, content);
	env_add(type, content, env, garb);
	return (0);
}

int	allocate_content(char **content, char *cmd, char *type)
{
	if (!env_len(cmd, 1))
	{
		*content = ft_strdup("");
		if (!*content)
		{
			free(type);
			return (1);
		}
	}
	else
	{
		*content = malloc(env_len(cmd, 1));
		if (!*content)
		{
			free(type);
			return (1);
		}
	}
	return (0);
}
