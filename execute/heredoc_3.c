/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:26:16 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:28:28 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char	*process_heredoc_node(t_ast **temp, t_env *env, t_garbage **garb,
		int first_heredoc)
{
	char	*heredoc_file;

	heredoc_file = NULL;
	if (first_heredoc)
		heredoc_file = create_heredoc_temp_file((*temp)->left, env, garb);
	else
		create_heredoc_temp_file((*temp)->left, env, garb);
	(*temp)->left = (*temp)->left->left;
	return (heredoc_file);
}

static void	process_non_heredoc_node(t_ast **temp, int *first_heredoc)
{
	*first_heredoc = 0;
	*temp = (*temp)->left;
}

char	*create_heredoc_if_needed(t_ast *node, t_env *env, t_garbage **garb)
{
	char	*heredoc_file;
	char	*temp_file;
	t_ast	*temp;
	int		first_heredoc;

	temp = node;
	heredoc_file = NULL;
	first_heredoc = 1;
	signal(SIGINT, SIG_IGN);
	while (temp && temp->left)
	{
		if (temp->left->type == NODE_HEREDOC)
		{
			temp_file = process_heredoc_node(&temp, env, garb, first_heredoc);
			if (first_heredoc && temp_file)
				heredoc_file = temp_file;
		}
		else
		{
			process_non_heredoc_node(&temp, &first_heredoc);
		}
	}
	set_parent_handlers();
	return (heredoc_file);
}

static void	setup_heredoc_fd(char *heredoc_file, t_ast *node)
{
	int	fd;

	if (!heredoc_file && node->left->type == NODE_REDIR_IN)
	{
		fd = open(node->left->filename, O_RDONLY);
		if (fd < 0)
		{
			perror("open heredoc file");
			exit(1);
		}
	}
	else
	{
		if (!heredoc_file)
			return ;
		fd = open(heredoc_file, O_RDONLY);
		if (fd < 0)
		{
			perror("open heredoc file");
			exit(1);
		}
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	execute_left_child(t_ast *node, t_env *env, int pipefd[2],
		t_post_order *post_order)
{
	int		result;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	setup_heredoc_fd(post_order->heredoc_file, node);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	result = execute(node->left, env, post_order->garb, &redir);
	g_free(post_order->garb);
	exit(result);
}
