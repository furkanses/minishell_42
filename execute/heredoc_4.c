/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:28:44 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 02:27:32 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

char	*create_heredoc_temp_file(t_ast *heredoc_node, t_env *env,
		t_garbage **garb)
{
	char	*temp_file;
	int		pipefd[2];
	pid_t	heredoc_pid;

	temp_file = create_temp_filename(garb);
	if (!temp_file)
		return (NULL);
	if (!setup_heredoc_pipe(pipefd, heredoc_node))
		return (NULL);
	heredoc_pid = fork();
	if (heredoc_pid == -1)
	{
		perror("fork heredoc");
		close(pipefd[0]);
		close(pipefd[1]);
		return (NULL);
	}
	if (heredoc_pid == 0)
		handle_child_process(heredoc_node, env, garb, pipefd);
	if (!handle_parent_process(pipefd, heredoc_pid))
		return (NULL);
	if (!write_pipe_to_file(temp_file, pipefd[0]))
		return (NULL);
	return (temp_file);
}

static int	check_heredoc_in_node(t_ast *node)
{
	t_ast	*temp;

	temp = node;
	while (temp)
	{
		if (temp->type == NODE_HEREDOC)
			return (1);
		if (temp->left)
			temp = temp->left;
		else
			break ;
	}
	return (0);
}

void	check_heredoc_status(t_ast *node, int *has_left, int *has_right)
{
	*has_left = check_heredoc_in_node(node->left);
	*has_right = check_heredoc_in_node(node->right);
}

void	handle_fake_heredocs(t_ast *node, t_env *env, t_garbage **garb,
		int append)
{
	t_ast			*temp;
	int				counter;
	t_post_order	post_order;

	post_order.append = append;
	post_order.redir = 0;
	post_order.output_file = node->filename;
	post_order.delimiter = 0;
	temp = node;
	counter = 0;
	while (temp && temp->left)
	{
		if (temp->left->type == 5 && counter > 0)
		{
			temp->left->fake_heredoc = 1;
			exec_heredoc_node(temp->left, env, garb, &post_order);
		}
		temp = temp->left;
		counter++;
	}
}

int	check_and_handle_heredocs(t_ast *node, t_env *env, t_post_order *post_order)
{
	t_ast		*temp;
	t_garbage	**garb;

	post_order->redir = 1;
	post_order->output_file = node->filename;
	post_order->delimiter = 1;
	temp = node;
	garb = post_order->garb;
	while (temp && temp->left && post_order->check == 0)
	{
		if (temp->left->type == 5)
			return (exec_heredoc_node(temp, env, garb, post_order));
		temp = temp->left;
	}
	return (-1);
}
