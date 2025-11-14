/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:04:48 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 10:08:32 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf/ft_printf.h"
#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	init_redir_struct(t_redir *redir, t_ast *node)
{
	redir->delimiter = 1;
	redir->redir_in = 1;
	redir->check = 0;
	set_multi_flags(node);
}

static void	handle_redir_in_check(t_ast *node, t_env *env, t_garbage **garb,
		t_redir *redir)
{
	if (node->left && node->left->type == NODE_REDIR_IN
		&& node->multi_flag == 0)
	{
		redir->check = 1;
		handle_fake_heredocs(node, env, garb, redir->append);
	}
}

int	exec_redirect_out(t_ast *node, t_env *env, int append, t_garbage **garb)
{
	t_post_order	post_order;
	t_redir			redir;

	redir.append = append;
	init_redir_struct(&redir, node);
	handle_redir_in_check(node, env, garb, &redir);
	post_order.garb = garb;
	post_order.append = append;
	post_order.check = redir.check;
	redir.heredoc_result = check_and_handle_heredocs(node, env, &post_order);
	if (redir.heredoc_result != -1)
		return (redir.heredoc_result);
	redir.fd = open_output_file(node, append);
	if (redir.fd < 0)
		return (0);
	if (node->multi_flag == 0)
		return (exec_with_fork(node, env, garb, redir.fd));
	else
	{
		close(redir.fd);
		return (execute(node->left, env, garb, &redir));
	}
}

int	process_heredocs(t_ast *node, t_env *env, t_garbage **garb, int *fake)
{
	t_ast	*temp;
	int		flag;
	t_redir	redir;

	redir.delimiter = 0;
	redir.redir_in = 1;
	temp = node;
	flag = 0;
	while (temp && temp->left)
	{
		if (temp->left->fake_heredoc == 1)
			*fake = 1;
		if (temp->left->type == NODE_HEREDOC && temp->left->fake_heredoc == 0)
		{
			flag = 1;
			temp->left->fake_heredoc = 1;
			execute(temp->left, env, garb, &redir);
		}
		temp = temp->left;
	}
	return (flag);
}

void	set_multi_redir_flags(t_ast *node)
{
	t_ast	*temp;

	temp = node;
	while (temp && temp->left)
	{
		temp->left->multi_redir = 1;
		temp = temp->left;
	}
}
