/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 02:21:58 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 06:21:24 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf/ft_printf.h"
#include "../minishell.h"
#include <stddef.h>

int	execute(t_ast *node, t_env *env, t_garbage **garb, t_redir *redir)
{
	t_post_order	post_order;

	post_order.append = 0;
	post_order.redir = 0;
	post_order.output_file = NULL;
	post_order.delimiter = redir->delimiter;
	if (!node)
		return (0);
	if (node->type == NODE_CMD && !get_env_cont_for_type(env, "PATH", 4)
		&& !is_builtin(node->args[0]))
	{
		ft_printf("minishell: %s: No such file or directory\n", node->args[0]);
		return (127);
	}
	if (node->type == NODE_HEREDOC)
		return (exec_heredoc_node(node, env, garb, &post_order));
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_REDIR_APPEND)
		return (exec_redirect_node(node, env, garb, redir->redir_in));
	else if (node->type == NODE_PIPE)
		return (exec_pipe_node(node, env, garb));
	else if (node->type == NODE_CMD)
		return (exec_cmd_node(node, env, garb));
	return (0);
}
