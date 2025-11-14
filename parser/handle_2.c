/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:29:52 by fses              #+#    #+#             */
/*   Updated: 2025/09/08 16:20:09 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int	handle_input_redirect(t_lexer *tokens)
{
	int	fd;

	if (!tokens->next || !tokens->next->str)
	{
		write(2, "minishell: filename required\n", 29);
		return (-1);
	}
	fd = open(tokens->next->str, O_RDONLY);
	if (fd < 0)
	{
		write(2, "minishell: no such file or directory: ", 39);
		write(2, tokens->next->str, ft_strlen(tokens->next->str));
		write(2, "\n", 1);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_redirect(t_lexer *tokens)
{
	int	fd;

	if (!tokens->next || !tokens->next->str)
	{
		write(2, "minishell: filename required\n", 29);
		return (-1);
	}
	fd = open(tokens->next->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open (redirect out)");
		return (-1);
	}
	close(fd);
	return (0);
}

void	handle_redir_node(t_ast **cmd_node, t_ast *redir_node, t_parser *parser)
{
	if (redir_node)
	{
		if (*cmd_node)
		{
			redir_node->left = *cmd_node;
			*cmd_node = redir_node;
		}
		else
		{
			*cmd_node = redir_node;
		}
		(*cmd_node)->delimiter_hr = parser->delimiter;
		(*cmd_node)->delimiter_hr_count = parser->delimiter_count;
		(*cmd_node)->files_rdr = parser->files;
		(*cmd_node)->files_count_rdr = parser->files_count;
	}
}
