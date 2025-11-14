/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:36:52 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 10:00:46 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

char	*create_temp_filename(t_garbage **garb)
{
	const char	*prefix;
	char		*pid_str;
	char		*temp_file;
	size_t		need;

	prefix = "/tmp/heredoc_";
	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (NULL);
	need = ft_strlen(prefix) + ft_strlen(pid_str) + 1;
	temp_file = malloc(need);
	if (!temp_file)
	{
		free(pid_str);
		return (NULL);
	}
	ft_strcpy(temp_file, prefix);
	ft_strcat(temp_file, pid_str);
	free(pid_str);
	g_collecter(garb, temp_file, 1);
	return (temp_file);
}

int	setup_heredoc_pipe(int pipefd[2], t_ast *heredoc_node)
{
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (0);
	}
	if (!heredoc_node->delimiter_hr)
	{
		write(2, "minishell: heredoc delimiter error\n", 35);
		close(pipefd[0]);
		close(pipefd[1]);
		return (0);
	}
	return (1);
}

static void	read_until_delimiter(char *key)
{
	char	*txt;

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
		free(txt);
	}
}

void	process_regular_delimiters(t_ast *heredoc_node, int *i)
{
	char	*key;

	while (*i < heredoc_node->delimiter_hr_count - 1
		&& heredoc_node->delimiter_hr[*i])
	{
		key = heredoc_node->delimiter_hr[*i];
		if (key)
			read_until_delimiter(key);
		if (global_access(0, 0))
			break ;
		(*i)++;
	}
}

char	*expand_if_needed(t_ast *heredoc_node, t_env *env, char *txt,
		t_garbage **garb)
{
	char	*expanded;

	if (heredoc_node->was_quoted != 6 && heredoc_node->was_quoted != 7
		&& ft_strchr(txt, '$'))
	{
		expanded = mini_expend_dollar(env, txt, garb);
		free(txt);
		return (expanded);
	}
	return (txt);
}
