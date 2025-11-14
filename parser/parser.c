/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:31:40 by fses              #+#    #+#             */
/*   Updated: 2025/09/07 13:12:17 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

static t_ast	*handle_token_type(t_lexer **tokens, t_ast **cmd_node,
		t_garbage **garb, t_lexer **last_heredoc)
{
	t_ast	*redir_node;

	if (!handle_heredoc_preprocessing(tokens, last_heredoc))
		return (NULL);
	redir_node = handle_redirection_tokens(tokens, garb);
	if (redir_node)
		return (redir_node);
	if (!redir_node)
		return (NULL);
	if ((*tokens)->token == 0)
	{
		if (!handle_word_token(tokens, cmd_node, garb))
			return (NULL);
	}
	else
	{
		ft_putstr_fd("minishell parser error: unexpected token\n", 2);
		return (NULL);
	}
	return (NULL);
}

static int	process_tokens_until_pipe(t_lexer **tokens, t_ast **cmd_node,
		t_lexer **last_heredoc, t_parser *parser)
{
	t_ast	*redir_node;

	while (*tokens && (*tokens)->token != PIPE)
	{
		redir_node = handle_token_type(tokens, cmd_node, parser->garb,
				last_heredoc);
		if (!redir_node && !*cmd_node)
			return (0);
		if (!redir_node)
			return (0);
		handle_redir_node(cmd_node, redir_node, parser);
		redir_node = NULL;
		while ((*tokens) && (*tokens)->token == 0)
		{
			if (add_argument_to_cmd((*cmd_node)->left, (*tokens)->str,
					parser->garb))
				return (0);
			*tokens = (*tokens)->next;
		}
	}
	return (1);
}

t_ast	*parse_command(t_lexer **tokens, t_garbage **garb)
{
	t_ast		*cmd_node;
	t_lexer		*temp;
	t_parser	parser;
	t_lexer		*last_heredoc;

	cmd_node = NULL;
	last_heredoc = NULL;
	if ((*tokens)->token == 0)
	{
		cmd_node = create_cmd_node(tokens, garb);
		if (!cmd_node)
			return (NULL);
	}
	parser.garb = garb;
	temp = *tokens;
	parser.delimiter_count = count_delimiters(temp);
	parser.files_count = count_output_files(temp);
	parser.files = create_files_array(*tokens, parser.files_count, garb);
	parser.delimiter = create_delimiter_array(*tokens, parser.delimiter_count,
			garb);
	if (!process_tokens_until_pipe(tokens, &cmd_node, &last_heredoc, &parser))
		return (NULL);
	return (cmd_node);
}

t_ast	*parse_pipeline(t_lexer **tokens, t_garbage **garb)
{
	t_ast	*left;
	t_ast	*pipe_node;

	left = parse_command(tokens, garb);
	if (!left)
		return (NULL);
	if (!(*tokens) || (*tokens)->token != PIPE)
		return (left);
	*tokens = (*tokens)->next;
	pipe_node = create_ast_node(NODE_PIPE, garb);
	if (!pipe_node)
		return (NULL);
	pipe_node->left = left;
	pipe_node->right = parse_pipeline(tokens, garb);
	if (!pipe_node->right)
	{
		ft_putstr_fd("minishell parser error: expected command after pipe\n",
			2);
		return (NULL);
	}
	return (pipe_node);
}

t_ast	*parse_tokens(t_lexer *tokens, t_garbage **garb)
{
	if (!tokens)
		return (NULL);
	return (parse_pipeline(&tokens, garb));
}
