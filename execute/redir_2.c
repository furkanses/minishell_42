/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:32:50 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:51:22 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include "../ft_printf/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int	open_input_file(t_ast *node)
{
	int	fd;

	fd = open(node->filename, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("minishell: no such file or directory: %s\n", node->filename);
		return (-1);
	}
	return (fd);
}

static int	execute_with_heredoc_logic(t_ast *node, t_env *env,
		t_garbage **garb, int flag)
{
	int		result;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	if (flag == 0)
	{
		if (node->left->fake_heredoc == 0)
			result = execute(node->left, env, garb, &redir);
		if (node->left->fake_heredoc == 1)
			result = execute(node->left->left, env, garb, &redir);
	}
	else
	{
		redir.delimiter = 0;
		redir.redir_in = 2;
		if (node->left->type == NODE_HEREDOC && node->left->left)
			result = execute(node->left->left, env, garb, &redir);
		else
			result = execute(node->left, env, garb, &redir);
	}
	return (result);
}

int	exec_with_fork_input(t_ast *node, t_env *env,
		t_post_order *post_order)
{
	pid_t	pid;
	int		status;
	int		result;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(post_order->fd);
		return (1);
	}
	if (pid == 0)
	{
		dup2(post_order->fd, STDIN_FILENO);
		close(post_order->fd);
		result = execute_with_heredoc_logic(node, env, post_order->garb,
				post_order->flag);
		g_free(post_order->garb);
		exit(result);
	}
	close(post_order->fd);
	waitpid(pid, &status, 0);
	return (ret_status(status));
}

int	execute_without_fork(t_ast *node, t_env *env, t_garbage **garb)
{
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	if (node->left->fake_heredoc == 0 || node->left->type != NODE_HEREDOC)
		return (execute(node->left, env, garb, &redir));
	if (node->left->fake_heredoc == 1 && node->left->type == NODE_HEREDOC)
		return (execute(node->left->left, env, garb, &redir));
	return (0);
}

int	open_output_file(t_ast *node, int append)
{
	int	fd;
	int	flags;

	flags = O_CREAT | O_WRONLY;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(node->filename, flags, 0644);
	if (fd < 0)
	{
		ft_printf("minishell: %s: Permission denied\n", node->filename);
		return (-1);
	}
	return (fd);
}
