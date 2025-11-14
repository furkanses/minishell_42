/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:27:58 by fses              #+#    #+#             */
/*   Updated: 2025/09/16 12:36:15 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>

t_ast	*create_ast_node(t_node_type type, t_garbage **garb)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!g_collecter(garb, node, 1))
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->filename = NULL;
	node->delimiter_hr = NULL;
	node->files_rdr = NULL;
	node->files_count_rdr = 0;
	node->is_expended = 0;
	node->delimiter_hr_count = 0;
	node->was_quoted = 0;
	node->multi_flag = 0;
	node->multi_redir = 0;
	node->left = NULL;
	node->right = NULL;
	node->fake_heredoc = 0;
	return (node);
}

char	**create_files_array(t_lexer *tokens, int files_count, t_garbage **garb)
{
	char	**files;
	t_lexer	*temp;
	int		id;

	if (files_count <= 0)
		return (NULL);
	files = (char **)malloc(sizeof(char *) * (files_count + 1));
	if (!g_collecter(garb, files, 1))
		return (NULL);
	temp = tokens;
	id = 0;
	while (temp && temp->token != PIPE && id < files_count)
	{
		if ((temp->token == 4 || temp->token == 2) && temp->next
			&& temp->next->token == 0)
		{
			files[id] = ft_strdup(temp->next->str);
			if (!g_collecter(garb, files[id], 1))
				return (NULL);
			id++;
		}
		temp = temp->next;
	}
	files[id] = NULL;
	return (files);
}

char	**create_delimiter_array(t_lexer *tokens, int delimiter_count,
		t_garbage **garb)
{
	char	**delimiter;
	t_lexer	*temp;
	int		idx;

	if (delimiter_count <= 0)
		return (NULL);
	delimiter = (char **)malloc(sizeof(char *) * (delimiter_count + 1));
	g_collecter(garb, delimiter, 1);
	if (!delimiter)
		return (NULL);
	temp = tokens;
	idx = 0;
	while (temp && temp->token != PIPE && idx < delimiter_count)
	{
		if (temp->token == DLESS && temp->next && temp->next->token == 0)
		{
			delimiter[idx] = ft_strdup(temp->next->str);
			g_collecter(garb, delimiter[idx], 1);
			idx++;
		}
		temp = temp->next;
	}
	delimiter[idx] = NULL;
	return (delimiter);
}

t_ast	*create_redir_node(t_node_type type, t_lexer **tokens, t_garbage **garb)
{
	t_ast	*node;

	if (!(*tokens) || !(*tokens)->next || (*tokens)->next->token != 0)
	{
		ft_putstr_fd("minishell parser error: \
            expected filename after redirection\n", 2);
		if (*tokens)
			*tokens = (*tokens)->next;
		return (NULL);
	}
	node = create_ast_node(type, garb);
	if (!node)
		return (NULL);
	*tokens = (*tokens)->next;
	node->filename = ft_strdup((*tokens)->str);
	if (!node->filename)
	{
		free(node);
		return (NULL);
	}
	g_collecter(garb, node->filename, 1);
	node->is_expended = (*tokens)->is_expended;
	node->was_quoted = (*tokens)->was_quoted;
	*tokens = (*tokens)->next;
	return (node);
}

t_ast	*create_cmd_node(t_lexer **tokens, t_garbage **garb)
{
	t_ast	*node;
	t_lexer	*current;
	int		arg_count;

	arg_count = count_command_args(*tokens);
	if (arg_count == 0)
		return (NULL);
	node = create_ast_node(NODE_CMD, garb);
	if (!node)
		return (NULL);
	node->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	if (!node->args)
	{
		free(node);
		return (NULL);
	}
	g_collecter(garb, node->args, 1);
	if (populate_args_array(node, *tokens, arg_count, garb) < 0)
		return (NULL);
	node->args[arg_count] = NULL;
	current = *tokens;
	while (arg_count-- > 0)
		current = current->next;
	*tokens = current;
	return (node);
}
