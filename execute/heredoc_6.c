/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_6.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:28:41 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:40:17 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <stdio.h>

int	check_redir_in(t_ast *node)
{
	t_ast	*temp;

	temp = node;
	while (temp)
	{
		if (temp->type == NODE_REDIR_APPEND || temp->type == NODE_REDIR_OUT)
			return (1);
		if (temp->left)
			temp = temp->left;
		else
			break ;
	}
	return (0);
}

static int	handle_double_heredoc_case(t_ast *node, t_env *env,
		t_garbage **garb)
{
	t_redir	redir;

	redir.delimiter = 0;
	redir.redir_in = 1;
	execute(node->left, env, garb, &redir);
	redir.delimiter = 1;
	return (execute(node->right, env, garb, &redir));
}

static int	handle_redirection_node_case(t_ast *node, t_env *env,
		t_garbage **garb)
{
	int		flag;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	execute(node->right, env, garb, &redir);
	if (node->left->type == NODE_REDIR_IN)
		flag = check_redir_in(node->left);
	else
		flag = 1;
	if (node->left && node->left->filename && flag == 1)
		return (execute(node->left, env, garb, &redir));
	return (1);
}

static int	handle_left_heredoc_case(t_ast *node, t_env *env, t_garbage **garb)
{
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	if (check_redir_in(node))
	{
		execute(node->left, env, garb, &redir);
		execute(node->right, env, garb, &redir);
		return (1);
	}
	return (-1);
}

int	handle_special_cases(t_ast *node, t_env *env, t_garbage **garb)
{
	int	has_left_heredoc;
	int	has_right_heredoc;

	check_heredoc_status(node, &has_left_heredoc, &has_right_heredoc);
	if (has_right_heredoc && has_left_heredoc)
		return (handle_double_heredoc_case(node, env, garb));
	if (is_redirection_node(node->right))
		return (handle_redirection_node_case(node, env, garb));
	if (has_left_heredoc)
		return (handle_left_heredoc_case(node, env, garb));
	return (-1);
}
