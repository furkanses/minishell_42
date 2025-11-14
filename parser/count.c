/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:26:50 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:36:11 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

int	populate_args_array(t_ast *node, t_lexer *current, int arg_count,
		t_garbage **garb)
{
	int	i;

	i = 0;
	while (i < arg_count)
	{
		node->args[i] = ft_strdup(current->str);
		if (!node->args[i])
		{
			while (--i >= 0)
				free(node->args[i]);
			free(node->args);
			free(node);
			return (-1);
		}
		g_collecter(garb, node->args[i], 1);
		current = current->next;
		i++;
	}
	return (0);
}

int	count_delimiters(t_lexer *tokens)
{
	int		delimiter_count;
	t_lexer	*temp;

	delimiter_count = 0;
	temp = tokens;
	while (temp && temp->token != PIPE)
	{
		if (temp->token == DLESS)
			delimiter_count++;
		temp = temp->next;
	}
	return (delimiter_count);
}

int	count_output_files(t_lexer *tokens)
{
	int		files_count;
	t_lexer	*temp;

	files_count = 0;
	temp = tokens;
	while (temp && temp->token != PIPE)
	{
		if (temp->token == GREAT || temp->token == DGREAT)
			files_count++;
		temp = temp->next;
	}
	return (files_count);
}

int	count_command_args(t_lexer *tokens)
{
	int		arg_count;
	t_lexer	*current;

	arg_count = 0;
	current = tokens;
	while (current && current->token == 0)
	{
		arg_count++;
		current = current->next;
	}
	return (arg_count);
}
