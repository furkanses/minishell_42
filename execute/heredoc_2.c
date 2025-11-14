/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 17:50:12 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 10:17:09 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

static int	handle_preliminary_delimiters(t_ast *heredoc_node,
		t_post_order *post_order, int pipefd[2])
{
	char	*key;
	int		i;

	i = 0;
	while ((i < heredoc_node->delimiter_hr_count - 1
			&& heredoc_node->delimiter_hr[i]) || post_order->delimiter == 0
		|| heredoc_node->fake_heredoc == 1)
	{
		key = heredoc_node->delimiter_hr[i];
		if (key)
			process_single_delimiter(key, post_order);
		if (global_access(0, 0))
		{
			g_free(post_order->garb);
			close(pipefd[1]);
			exit(130);
		}
		if (!key)
			break ;
		i++;
	}
	return (0);
}

static int	handle_readline_and_exit_conditions(char **txt, char *key,
		t_post_order *post_order)
{
	*txt = readline("> ");
	if (global_access(0, 0) || !(*txt) || ft_strcmp(*txt, key) == 0)
	{
		if (global_access(0, 0))
			g_free(post_order->garb);
		free(*txt);
		return (1);
	}
	return (0);
}

static void	process_heredoc_input_loop(t_ast *heredoc_node, t_env *env,
		t_post_order *post_order, int pipefd[2])
{
	char	*txt;
	char	*expanded;

	while (1)
	{
		if (handle_readline_and_exit_conditions(&txt, post_order->key,
				post_order))
			break ;
		if (heredoc_node->was_quoted != 6 && heredoc_node->was_quoted != 7
			&& ft_strchr(txt, '$'))
		{
			expanded = mini_expend_dollar(env, txt, post_order->garb);
			free(txt);
			txt = expanded;
		}
		if (ft_strlen(txt) > 0)
			write(pipefd[1], txt, ft_strlen(txt));
		write(pipefd[1], "\n", 1);
		free(txt);
	}
}

static void	process_final_delimiter(t_ast *heredoc_node, t_env *env,
		t_post_order *post_order, int pipefd[2])
{
	if (heredoc_node->delimiter_hr[heredoc_node->delimiter_hr_count - 1]
		&& post_order->delimiter == 1)
	{
		post_order->key = heredoc_node->delimiter_hr[
			heredoc_node->delimiter_hr_count - 1];
		process_heredoc_input_loop(heredoc_node, env, post_order, pipefd);
	}
}

void	heredoc_child_process(int pipefd[2], t_ast *heredoc_node, t_env *env,
		t_post_order *post_order)
{
	global_access(1, 0);
	signal(SIGINT, heredoc_interrupt_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	handle_preliminary_delimiters(heredoc_node, post_order, pipefd);
	process_final_delimiter(heredoc_node, env, post_order, pipefd);
	close(pipefd[1]);
	if (global_access(0, 0))
	{
		g_free(post_order->garb);
		exit(130);
	}
	g_free(post_order->garb);
	exit(0);
}
