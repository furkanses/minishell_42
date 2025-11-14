/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:13:50 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:34:41 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int	wait_processes_and_cleanup(pid_t pid1, pid_t pid2,
		char *heredoc_file, int pipefd[2])
{
	int	status;

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (heredoc_file)
		unlink(heredoc_file);
	return (ret_status(status));
}

static void	handle_left_node_execution(t_ast *node, t_env *env,
		t_garbage **garb)
{
	t_redir	redir;

	if (node->left && check_redir_in(node->left))
	{
		redir.delimiter = 1;
		redir.redir_in = 1;
		execute(node->left, env, garb, &redir);
	}
	else if (node->left && node->left->filename)
	{
		redir.delimiter = 0;
		redir.redir_in = 0;
		execute(node->left, env, garb, &redir);
	}
}

static int	process_pipe_redirection_chain(t_ast *node, t_env *env,
		t_garbage **garb)
{
	t_ast	*temp;
	t_redir	redir;

	temp = node;
	while (temp->right->type == NODE_PIPE && temp->right)
	{
		temp = temp->right;
		if (is_redirection_node(temp->right) && temp->right->type != NODE_PIPE)
		{
			handle_left_node_execution(node, env, garb);
			redir.delimiter = 1;
			redir.redir_in = 1;
			return (execute(node->right, env, garb, &redir));
		}
	}
	return (-1);
}

static int	handle_pipe_preparations(t_ast *node, t_env *env, t_garbage **garb,
		t_redir *redir)
{
	int	pipe_result;
	int	special_result;

	global_access(1, 0);
	pipe_result = process_pipe_redirection_chain(node, env, garb);
	if (pipe_result != -1)
		return (pipe_result);
	special_result = handle_special_cases(node, env, garb);
	if (special_result != -1)
		return (special_result);
	redir->heredoc_file = create_heredoc_if_needed(node, env, garb);
	if (node->left && node->left->type == NODE_HEREDOC && !redir->heredoc_file)
		return (130);
	return (-1);
}

int	exec_pipe_node(t_ast *node, t_env *env, t_garbage **garb)
{
	int				pipefd[2];
	int				prep_result;
	t_post_order	post_order;
	t_redir			redir;

	prep_result = handle_pipe_preparations(node, env, garb, &redir);
	if (prep_result != -1)
		return (prep_result);
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (1);
	}
	post_order.garb = garb;
	post_order.heredoc_file = redir.heredoc_file;
	redir.pid1 = fork_left_process(node, env, &post_order, pipefd);
	if (redir.pid1 == -1)
		return (1);
	post_order.garb = garb;
	post_order.heredoc_file = redir.heredoc_file;
	redir.pid2 = fork_right_process(node, env, &post_order, pipefd);
	if (redir.pid2 == -1)
		return (1);
	return (wait_processes_and_cleanup(redir.pid1, redir.pid2,
			redir.heredoc_file, pipefd));
}
