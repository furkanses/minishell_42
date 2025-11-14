/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:59:49 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:32:43 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf/ft_printf.h"
#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static int	handle_redir_in_type_2(t_ast *node, t_env *env, t_garbage **garb,
		t_redir *redir)
{
	redir->delimiter = 0;
	redir->redir_in = 2;
	if (node->left->type == NODE_HEREDOC && node->left->left)
		return (execute(node->left->left, env, garb, redir));
	if (node->left && node->left->type != NODE_CMD)
		return (execute(node->left, env, garb, redir));
	return (-1);
}

static int	handle_redir_in_type_0(t_ast *node, t_env *env, t_garbage **garb,
		t_redir *redir)
{
	redir->delimiter = 0;
	redir->redir_in = 0;
	if (node->left && node->left->type != NODE_CMD)
		return (execute(node->left, env, garb, redir));
	return (1);
}

static int	handle_fork_input_execution(t_ast *node, int fd, int flag,
		t_dollar *dollar)
{
	t_post_order	post_order;

	post_order.fd = fd;
	post_order.flag = flag;
	post_order.garb = dollar->garb;
	return (exec_with_fork_input(node, dollar->env, &post_order));
}

static int	process_initial_checks_and_setup(t_dollar *dollar, int *flag,
		int *fake, int *fd)
{
	int		result;
	t_redir	redir;

	if (dollar->redir_in == 2)
	{
		result = handle_redir_in_type_2(dollar->node, dollar->env, dollar->garb,
				&redir);
		if (result != -1)
			return (result);
	}
	if (dollar->redir_in == 0)
		return (handle_redir_in_type_0(dollar->node, dollar->env, dollar->garb,
				&redir));
	*fake = 0;
	*flag = process_heredocs(dollar->node, dollar->env, dollar->garb, fake);
	set_multi_redir_flags(dollar->node);
	*fd = open_input_file(dollar->node);
	return (-1);
}

int	exec_redirect_in(t_ast *node, t_env *env, t_garbage **garb, int redir_in)
{
	int			fd;
	int			flag;
	int			fake;
	int			result;
	t_dollar	dollar;

	dollar.node = node;
	dollar.env = env;
	dollar.garb = garb;
	dollar.redir_in = redir_in;
	result = process_initial_checks_and_setup(&dollar, &flag, &fake, &fd);
	if (result != -1)
		return (result);
	if (fd < 0)
		return (1);
	if ((node->multi_redir == 0 && node->delimiter_hr_count == 0) || flag == 1
		|| fake == 1)
		return (handle_fork_input_execution(node, fd, flag, &dollar));
	else
	{
		close(fd);
		return (execute_without_fork(node, env, garb));
	}
}
