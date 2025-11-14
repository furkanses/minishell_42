/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_punctuation.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:08:21 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:10:28 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static char	**env_list_to_array(t_env *env, t_garbage **garb)
{
	int		count;
	char	**arr;

	count = count_env_list(env);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	g_collecter(garb, arr, 1);
	if (fill_env_array(arr, env, garb) == -1)
		return (NULL);
	arr[count] = NULL;
	return (arr);
}

static void	sort_str_array(char **arr)
{
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	while (arr[i])
	{
		j = i + 1;
		while (arr[j])
		{
			if (ft_strncmp(arr[i], arr[j], ft_strlen(arr[i])) > 0)
			{
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	export_out(t_env *env, t_garbage **garb)
{
	char	**arr;
	int		i;

	arr = env_list_to_array(env, garb);
	if (!arr)
		return (1);
	sort_str_array(arr);
	i = -1;
	while (arr[++i])
	{
		if (ft_strncmp("declare -x _", arr[i], 12) != 0)
		{
			write(1, arr[i], ft_strlen(arr[i]));
			write(1, "\n", 1);
		}
	}
	return (0);
}

int	builtin_pwd(t_garbage **garb, t_env *env)
{
	(void)garb;
	printf("%s\n", get_env_cont_for_type(env, "PWD", 3));
	return (0);
}

int	builtin_punctuation(char **args)
{
	if (ft_strcmp(args[0], ".") == 0)
	{
		ft_putstr_fd("minishell: .: filename argument \
            required\n.: usage: . filename [arguments]\n",
			2);
		return (2);
	}
	if (ft_strcmp(args[0], "") == 0)
	{
		ft_putstr_fd("Command '' not found\n", 2);
		return (127);
	}
	if (ft_strcmp(args[0], "./") == 0)
	{
		ft_putstr_fd("minishell: ./: Is a directory\n", 2);
		return (126);
	}
	if (ft_strcmp(args[0], "/") == 0)
	{
		ft_putstr_fd("minishell: ./: Is a directory\n", 2);
		return (126);
	}
	return (1);
}
