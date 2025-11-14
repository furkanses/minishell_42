/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:32:07 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:30:37 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	setup_heredoc_child_io(int *pipefd, t_ast *node,
		t_post_order *post_order)
{
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	handle_output_redirection(node, post_order);
}

static void	execute_heredoc_command(t_ast *heredoc_node, t_env *env,
		t_garbage **garb)
{
	int		result;
	t_redir	redir;

	if (heredoc_node->fake_heredoc == 1)
		result = 1;
	if (heredoc_node->left && heredoc_node->fake_heredoc == 0)
		result = execute(heredoc_node->left, env, garb, &redir);
	if (heredoc_node->left && heredoc_node->left->type == NODE_CMD)
	{
		result = execute(heredoc_node->left, env, garb, &redir);
		g_free(garb);
		exit(result);
	}
	else
		result = 0;
	g_free(garb);
	exit(result);
}

static void	handle_command_child_process(t_ast *node, t_env *env,
		t_exec_context *exec)
{
	setup_heredoc_child_io(exec->pipefd, node, exec->post_order);
	execute_heredoc_command(exec->heredoc_node, env, exec->garb);
}

static int	fork_and_execute_command(t_ast *node, t_env *env,
		t_exec_context *exec)
{
	pid_t	cmd_pid;
	int		status;

	cmd_pid = fork();
	if (cmd_pid == -1)
	{
		perror("fork command");
		close(exec->pipefd[0]);
		return (1);
	}
	if (cmd_pid == 0)
	{
		handle_command_child_process(node, env, exec);
	}
	close(exec->pipefd[0]);
	waitpid(cmd_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	exec_heredoc_node(t_ast *node, t_env *env, t_garbage **garb,
		t_post_order *post_order)
{
	int				pipefd[2];
	t_ast			*heredoc_node;
	pid_t			heredoc_pid;
	t_exec_context	exec;
	int				heredoc_result;

	if (pipe(pipefd) == -1)
		return (1);
	heredoc_node = find_heredoc_node(node, post_order);
	if (validate_and_setup_heredoc(heredoc_node, pipefd) != 0)
		return (1);
	heredoc_pid = create_heredoc_process(pipefd);
	if (heredoc_pid == -1)
		return (1);
	post_order->garb = garb;
	if (heredoc_pid == 0)
		heredoc_child_process(pipefd, heredoc_node, env, post_order);
	heredoc_result = heredoc_parent_process(heredoc_pid, pipefd);
	if (heredoc_result == 130)
		return (130);
	exec.pipefd = pipefd;
	exec.post_order = post_order;
	exec.heredoc_node = heredoc_node;
	exec.garb = garb;
	return (fork_and_execute_command(node, env, &exec));
}
