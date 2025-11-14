/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 09:30:00 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:51:43 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

void	process_single_delimiter(char *key, t_post_order *post_order)
{
	char	*txt;

	while (1)
	{
		txt = readline("> ");
		if (global_access(0, 0) || !txt || ft_strcmp(txt, key) == 0)
		{
			if (global_access(0, 0))
				g_free(post_order->garb);
			free(txt);
			if (post_order->delimiter == 0)
				post_order->delimiter = 2;
			break ;
		}
		free(txt);
	}
}

static void	create_intermediate_files(t_ast *node)
{
	int	i;
	int	fd;

	i = 0;
	while (i < node->files_count_rdr - 1)
	{
		fd = open(node->files_rdr[i], O_CREAT, 0644);
		if (fd < 0)
		{
			perror("dosya oluşturulamadı");
			exit(1);
		}
		close(fd);
		i++;
	}
}

static void	setup_output_redirection(t_post_order *post_order)
{
	int	flags;
	int	outfd;

	flags = O_CREAT | O_WRONLY;
	if (post_order->append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	outfd = open(post_order->output_file, flags, 0644);
	if (outfd < 0)
	{
		perror("open (heredoc redirect)");
		exit(1);
	}
	dup2(outfd, STDOUT_FILENO);
	close(outfd);
}

void	handle_output_redirection(t_ast *node, t_post_order *post_order)
{
	if (post_order->redir == 1 && post_order->output_file)
	{
		create_intermediate_files(node);
		setup_output_redirection(post_order);
	}
}
