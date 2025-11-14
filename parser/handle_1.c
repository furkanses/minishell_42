/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:29:30 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 10:09:33 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

int	handle_word_token(t_lexer **tokens, t_ast **cmd_node, t_garbage **garb)
{
	if (!*cmd_node)
	{
		*cmd_node = create_cmd_node(tokens, garb);
		if (!*cmd_node)
			return (0);
	}
	else
	{
		ft_putstr_fd("minishell parser error: unexpected word token\n", 2);
		return (0);
	}
	return (1);
}

t_ast	*handle_redirection_tokens(t_lexer **tokens, t_garbage **garb)
{
	t_ast	*redir_node;

	redir_node = NULL;
	if ((*tokens)->token == LESS)
		redir_node = create_redir_node(NODE_REDIR_IN, tokens, garb);
	else if ((*tokens)->token == GREAT)
		redir_node = create_redir_node(NODE_REDIR_OUT, tokens, garb);
	else if ((*tokens)->token == DGREAT)
		redir_node = create_redir_node(NODE_REDIR_APPEND, tokens, garb);
	else if ((*tokens)->token == DLESS)
		redir_node = create_redir_node(NODE_HEREDOC, tokens, garb);
	return (redir_node);
}

int	handle_heredoc_preprocessing(t_lexer **tokens, t_lexer **last_heredoc)
{
	if ((*tokens) && (*tokens)->token == DLESS)
	{
		*last_heredoc = handle_heredoc_sequence(*tokens);
		if (!*last_heredoc)
			return (0);
		if (*last_heredoc != *tokens)
			*tokens = *last_heredoc;
	}
	return (1);
}

t_lexer	*handle_heredoc_sequence(t_lexer *tokens)
{
	t_lexer	*last_heredoc;
	t_lexer	*current;

	last_heredoc = tokens;
	current = tokens->next;
	if (!current || current->token != 0)
	{
		ft_putstr_fd("minishell error: expected filename after heredoc\n",
			2);
		return (NULL);
	}
	while (current->next && current->next->token != PIPE)
	{
		if (current->next->token == DLESS && current->next->next
			&& current->next->next->token == 0)
		{
			last_heredoc = current->next;
			current = current->next->next;
		}
		else
			current = current->next;
	}
	return (last_heredoc);
}
