/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:32:55 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 01:26:13 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void	write_line_to_pipe(char *txt, int pipefd)
{
	txt = ft_strdup(txt);
	if (ft_strlen(txt) > 0)
		write(pipefd, txt, ft_strlen(txt));
	write(pipefd, "\n", 1);
	free(txt);
}

static void	process_last_delimiter(t_ast *heredoc_node, t_env *env,
		t_garbage **garb, int pipefd)
{
	char	*txt;
	char	*key;

	key = heredoc_node->delimiter_hr[heredoc_node->delimiter_hr_count - 1];
	while (1)
	{
		txt = readline("> ");
		if (global_access(0, 0) || !txt)
			break ;
		if (ft_strcmp(txt, key) == 0)
		{
			free(txt);
			break ;
		}
		txt = expand_if_needed(heredoc_node, env, txt, garb);
		write_line_to_pipe(txt, pipefd);
	}
}

void	handle_child_process(t_ast *heredoc_node, t_env *env, t_garbage **garb,
		int pipefd[2])
{
	int	i;

	i = 0;
	close(pipefd[0]);
	signal(SIGINT, heredoc_interrupt_handler);
	signal(SIGQUIT, SIG_IGN);
	process_regular_delimiters(heredoc_node, &i);
	if (heredoc_node->delimiter_hr[heredoc_node->delimiter_hr_count - 1]
		&& !global_access(0, 0))
		process_last_delimiter(heredoc_node, env, garb, pipefd[1]);
	close(pipefd[1]);
	if (global_access(0, 0))
	{
		g_free(garb);
		exit(130);
	}
	g_free(garb);
	exit(0);
}

int	handle_parent_process(int pipefd[2], pid_t heredoc_pid)
{
	int	status;

	close(pipefd[1]);
	waitpid(heredoc_pid, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		close(pipefd[0]);
		return (0);
	}
	return (1);
}

int	write_pipe_to_file(char *temp_file, int pipefd)
{
	int		fd;
	char	buffer[1024];
	ssize_t	bytes_read;

	fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open temp file");
		close(pipefd);
		return (0);
	}
	bytes_read = read(pipefd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(fd, buffer, bytes_read);
		bytes_read = read(pipefd, buffer, sizeof(buffer));
	}
	close(fd);
	close(pipefd);
	return (1);
}
