/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:11:41 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:19:53 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

static void	format_env_string(char *str, size_t size, t_env *env_node)
{
	str[0] = '\0';
	ft_strlcat(str, "declare -x ", size);
	ft_strlcat(str, env_node->type, size);
	if (env_node->content != NULL)
	{
		ft_strlcat(str, "=\"", size);
		ft_strlcat(str, env_node->content, size);
		ft_strlcat(str, "\"", size);
	}
}

int	fill_env_array(char **arr, t_env *env, t_garbage **garb)
{
	t_env	*tmp;
	int		i;
	size_t	len;

	tmp = env;
	i = 0;
	while (tmp)
	{
		len = calculate_env_string_length(tmp);
		arr[i] = malloc(len + 1);
		if (!arr[i])
			return (-1);
		g_collecter(garb, arr[i], 1);
		format_env_string(arr[i], len + 1, tmp);
		i++;
		tmp = tmp->next;
	}
	return (0);
}

static unsigned char	ex_atoi(const char *str)
{
	long	val;
	int		sign;

	val = 0;
	sign = 1;
	while (is_ws((unsigned char)*str))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (ft_isdigit((unsigned char)*str))
	{
		val = val * 10 + (*str - '0');
		str++;
	}
	if (sign < 0)
		val = -val;
	return ((unsigned char)(val & 0xFF));
}

static int	is_numeric(const char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	while (*str && is_ws((unsigned char)*str))
		str++;
	if (*str == '\0')
		return (0);
	if (*str == '+' || *str == '-')
		str++;
	if (*str == '\0')
		return (0);
	i = 0;
	while (str[i] && ft_isdigit((unsigned char)str[i]))
		i++;
	while (str[i] && is_ws((unsigned char)str[i]))
		i++;
	return (str[i] == '\0');
}

int	builtin_exit(char **cmd)
{
	if (!cmd[1])
		return (2);
	else if (is_numeric(cmd[1]) && !cmd[2])
		return (ex_atoi(cmd[1]));
	else if (!is_numeric(cmd[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(": requires ", 2);
		ft_putstr_fd(cmd[1], 2);
		ft_putstr_fd(" numeric argument\n", 2);
		return (2);
	}
	else if (is_numeric(cmd[1]) && cmd[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1000);
	}
	return (0);
}
