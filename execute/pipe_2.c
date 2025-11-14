/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:34:48 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 10:03:20 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "../libft/libft.h"

pid_t	fork_left_process(t_ast *node, t_env *env,
		t_post_order *post_order, int pipefd[2])
{
	pid_t	pid1;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork (left)");
		close(pipefd[0]);
		close(pipefd[1]);
		if (post_order->heredoc_file)
			unlink(post_order->heredoc_file);
		return (-1);
	}
	if (pid1 == 0)
		execute_left_child(node, env, pipefd, post_order);
	return (pid1);
}

static void	execute_right_child(t_ast *node, t_env *env, t_garbage **garb,
		int pipefd[2])
{
	int		result;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	result = execute(node->right, env, garb, &redir);
	g_free(garb);
	exit(result);
}

pid_t	fork_right_process(t_ast *node, t_env *env,
		t_post_order *post_order, int pipefd[2])
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork (right)");
		close(pipefd[0]);
		close(pipefd[1]);
		if (post_order->heredoc_file)
			unlink(post_order->heredoc_file);
		return (-1);
	}
	if (pid2 == 0)
		execute_right_child(node, env, post_order->garb, pipefd);
	return (pid2);
}

int	exec_with_fork(t_ast *node, t_env *env, t_garbage **garb, int fd)
{
	pid_t	pid;
	int		status;
	int		result;
	t_redir	redir;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(fd);
		return (1);
	}
	if (pid == 0)
	{
		redir.delimiter = 1;
		redir.redir_in = 1;
		dup2(fd, STDOUT_FILENO);
		close(fd);
		result = execute(node->left, env, garb, &redir);
		g_free(garb);
		exit(result);
	}
	close(fd);
	waitpid(pid, &status, 0);
	return (ret_status(status));
}

char	*normalize_path(const char *path, t_garbage **garb)
{
	size_t	j;
	size_t	n;
	size_t	cap;
	char	*out;

	if (!path)
		return (NULL);
	n = ft_strlen(path);
	cap = (n * 2) + 3;
	out = (char *)malloc(cap);
	if (!out)
		return (NULL);
	g_collecter(garb, out, 1);
	j = process_path_normalization(path, out, n);
	if (n > 0 && path[n - 1] == ':')
		out[j++] = '.';
	out[j] = '\0';
	return (out);
}
