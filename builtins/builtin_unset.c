/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:33:37 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:10:11 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

int	env_remove(char *type, t_env **env)
{
	t_env	*tmp;
	t_env	*prev;

	tmp = *env;
	prev = NULL;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, type, ft_strlen(tmp->type)) == 0)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*env = tmp->next;
			return (0);
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return (1);
}

int	env_add(char *type, char *content, t_env **env, t_garbage **garb)
{
	t_env	*new;
	t_env	*tmp;

	new = malloc(sizeof(t_env));
	if (!g_collecter_env(garb, new))
		return (1);
	new->type = type;
	new->content = content;
	new->next = NULL;
	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (0);
}

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !(ft_isalpha((unsigned char)*s) || *s == '_'))
		return (0);
	i = 0;
	while (s[++i])
		if (!(ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
			return (0);
	return (1);
}

static int	unset_env_var(char *name, t_env **env)
{
	t_env	*iter;
	t_env	*prev;

	if (!is_valid_identifier(name))
		return (1);
	iter = *env;
	prev = NULL;
	while (iter && ft_strncmp(name, iter->type, ft_strlen(name)) != 0)
	{
		prev = iter;
		iter = iter->next;
	}
	if (!iter)
		return (0);
	if (prev)
		prev->next = iter->next;
	else
		*env = iter->next;
	return (0);
}

int	builtin_unset(char **cmd, t_env **env)
{
	int	status;
	int	i;

	status = 0;
	if (!cmd || !env || !(*env))
		return (1);
	i = 0;
	while (cmd[++i])
	{
		if (unset_env_var(cmd[i], env) != 0)
			status = 1;
	}
	return (status);
}
