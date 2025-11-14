/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:18:48 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 00:55:50 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>

int	count_env_list(t_env *env)
{
	int		count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

size_t	calculate_env_string_length(t_env *env_node)
{
	size_t	base;
	size_t	len;

	base = ft_strlen("declare -x ") + ft_strlen(env_node->type);
	if (env_node->content == NULL)
		len = base;
	else if (env_node->content[0] == '\0')
		len = base + 3;
	else
		len = base + 2 + ft_strlen(env_node->content) + 1;
	return (len);
}

int	builtin_env(char **cmd, t_env *env)
{
	t_env	*iter;

	if (cmd[1])
	{
		ft_putstr_fd("env argument error", 2);
		return (1);
	}
	iter = env;
	while (iter)
	{
		if (iter->content)
			printf("%s=%s\n", iter->type, iter->content);
		iter = iter->next;
	}
	return (0);
}

static int	is_n_arg(char *arg)
{
	int	i;

	i = 1;
	if (ft_strncmp("-n", arg, 2) != 0)
		return (0);
	while (arg[++i])
		if (arg[i] != 'n')
			return (0);
	return (1);
}

int	builtin_echo(char **cmd)
{
	int	i;
	int	c;

	c = 0;
	i = 1;
	if (cmd[1] == NULL)
	{
		ft_putchar_fd('\n', 1);
		return (0);
	}
	while (cmd[i] && is_n_arg(cmd[i]))
	{
		i++;
		c++;
	}
	while (cmd[i])
	{
		ft_putstr_fd(cmd[i], 1);
		if (cmd[++i])
			ft_putchar_fd(' ', 1);
	}
	if (!c)
		ft_putchar_fd('\n', 1);
	return (0);
}
