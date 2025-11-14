/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fses <fses@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:09:43 by fses              #+#    #+#             */
/*   Updated: 2025/08/31 13:16:09 by fses             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

int	env_len(char *env, int t)
{
	int	l;
	int	i;

	i = 0;
	l = 0;
	while (env[i] != '=')
		i++;
	if (t)
	{
		while (env[i + l])
			l++;
		return (l);
	}
	return (i + 1);
}

void	env_cpy(char *type, char *content, const char *env)
{
	int	i;
	int	l;

	l = 0;
	i = -1;
	while (env[++i] != '=')
		type[i] = env[i];
	type[i] = 0;
	i++;
	while (env[i + l])
	{
		content[l] = env[i + l];
		l++;
	}
	content[l] = 0;
}

t_env	*init_env(t_local *local, t_garbage **garb)
{
	local->root_env = malloc(sizeof(t_env));
	if (!local->root_env)
		return (NULL);
	g_collecter_env(garb, local->root_env);
	local->root_env->type = NULL;
	local->root_env->content = NULL;
	local->root_env->next = NULL;
	local->iter_env = local->root_env;
	local->i = 0;
	return (local->root_env);
}

int	handle_next_env_node(char **env, t_local *local, t_garbage **garb)
{
	if (env[++local->i])
	{
		local->iter_env->next = malloc(sizeof(t_env));
		if (!local->iter_env->next)
			return (0);
		g_collecter_env(garb, local->iter_env->next);
		local->iter_env->next->type = NULL;
		local->iter_env->next->content = NULL;
		local->iter_env->next->next = NULL;
		local->iter_env = local->iter_env->next;
		return (1);
	}
	else
	{
		local->iter_env->next = NULL;
		return (1);
	}
}

t_env	*split_env(char **env, t_garbage **garb)
{
	t_local	local;

	if (!init_env(&local, garb))
		return (NULL);
	while (env[local.i])
	{
		local.iter_env->type = malloc(env_len(env[local.i], 0));
		if (!local.iter_env->type)
			return (NULL);
		g_collecter_env(garb, local.iter_env->type);
		local.iter_env->content = malloc(env_len(env[local.i], 1));
		if (!local.iter_env->content)
			return (NULL);
		g_collecter_env(garb, local.iter_env->content);
		env_cpy(local.iter_env->type, local.iter_env->content, env[local.i]);
		if (!handle_next_env_node(env, &local, garb))
			return (NULL);
	}
	return (local.root_env);
}
