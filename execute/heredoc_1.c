/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:44:44 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:25:38 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int	exec_redirect_node(t_ast *node, t_env *env, t_garbage **garb, int redir_in)
{
	if (node->type == NODE_REDIR_OUT)
		return (exec_redirect_out(node, env, 0, garb));
	else if (node->type == NODE_REDIR_IN)
		return (exec_redirect_in(node, env, garb, redir_in));
	else if (node->type == NODE_REDIR_APPEND)
		return (exec_redirect_out(node, env, 1, garb));
	return (0);
}

int	heredoc_parent_process(pid_t heredoc_pid, int pipefd[2])
{
	int	status;

	close(pipefd[1]);
	waitpid(heredoc_pid, &status, 0);
	set_parent_handlers();
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		return (130);
	}
	return (0);
}

t_ast	*find_heredoc_node(t_ast *node, t_post_order *post_order)
{
	t_ast	*heredoc_node;
	t_ast	*temp;

	heredoc_node = node;
	if (post_order->redir == 1)
	{
		temp = node;
		while (temp && temp->left)
		{
			temp = temp->left;
			if (temp->type == NODE_HEREDOC)
				heredoc_node = temp;
		}
	}
	return (heredoc_node);
}

int	validate_and_setup_heredoc(t_ast *heredoc_node, int *pipefd)
{
	if (!heredoc_node->delimiter_hr)
	{
		write(2, "minishell: heredoc delimiter error\n", 35);
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	signal(SIGINT, SIG_IGN);
	return (0);
}

pid_t	create_heredoc_process(int *pipefd)
{
	pid_t	heredoc_pid;

	heredoc_pid = fork();
	if (heredoc_pid == -1)
	{
		perror("fork heredoc");
		close(pipefd[0]);
		close(pipefd[1]);
		return (-1);
	}
	return (heredoc_pid);
}
