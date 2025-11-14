/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:33:24 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:35:39 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int	file_check(t_lexer *tokens)
{
	int	fd;

	while (tokens)
	{
		if (tokens->token == 1)
		{
			fd = open(tokens->next->str, O_RDONLY);
			if (fd < 0)
				return (0);
			close(fd);
		}
		if (tokens->next)
			tokens = tokens->next;
		else
			break ;
	}
	return (1);
}

int	punctuation_tool(char *str, int *flag)
{
	ft_putstr_fd(str, 2);
	*flag += 2;
	return (0);
}

int	punctuation(t_lexer *tokens, int *flag)
{
	*flag = 0;
	if (ft_strcmp(tokens->str, ".") == 0)
		return (punctuation_tool("minishell: .: filename argument \
            required\n.: usage: . filename [arguments]\n",
				flag));
	if (ft_strcmp(tokens->str, "") == 0)
		return (punctuation_tool("Command '' not found\n", flag));
	if (ft_strcmp(tokens->str, "./") == 0)
		return (punctuation_tool("minishell: ./: Is a directory\n", flag));
	if (ft_strcmp(tokens->str, "/") == 0)
		return (punctuation_tool("minishell: ./: Is a directory\n", flag));
	return (1);
}

int	syntax_check(t_lexer *tokens, int *flag)
{
	*flag = 0;
	if (!punctuation(tokens, flag))
		return (0);
	if (tokens->token == 3)
	{
		*flag = 1;
		return (0);
	}
	while (tokens)
	{
		if (tokens->token == 4 || tokens->token == 1 || tokens->token == 5
			|| tokens->token == 2 || tokens->token == 3)
		{
			if (tokens->token == 3)
				*flag = 1;
			if (!tokens->next || tokens->next->token != 0)
				return (0);
		}
		if (tokens->next)
			tokens = tokens->next;
		else
			break ;
	}
	return (1);
}

int	add_argument_to_cmd(t_ast *cmd_node, char *arg, t_garbage **garb)
{
	int		arg_count;
	char	**new_args;
	int		i;

	if (!cmd_node || !cmd_node->args || !arg)
		return (1);
	arg_count = 0;
	while (cmd_node->args[arg_count])
		arg_count++;
	new_args = (char **)malloc(sizeof(char *) * (arg_count + 2));
	if (!g_collecter(garb, new_args, 1))
		return (1);
	i = -1;
	while (++i < arg_count)
		new_args[i] = cmd_node->args[i];
	new_args[arg_count] = ft_strdup(arg);
	if (!g_collecter(garb, new_args[arg_count], 1))
		return (1);
	new_args[arg_count + 1] = NULL;
	cmd_node->args = new_args;
	return (0);
}
