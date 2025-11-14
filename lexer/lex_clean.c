/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_clean.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:06:05 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:11:17 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

void	assign_lex_cpy(t_local *local, int *was_quoted_ptr, int *is_expended,
		int i)
{
	local->l = 0;
	local->q = 0;
	local->start_i = i;
	*was_quoted_ptr = 0;
	*is_expended = 0;
}

int	handle_escape_char(char *dst, char *src, int *i, int *l)
{
	if (src[*i] == '\\' && src[*i + 1])
	{
		(*i)++;
		dst[(*l)++] = src[(*i)++];
		return (1);
	}
	return (0);
}

int	token(char *str)
{
	if (str[0] == '<' && str[1] == 0)
		return (1);
	else if (str[0] == '<' && str[1] == '<' && str[2] == 0)
		return (2);
	else if (str[0] == '|' && str[1] == 0)
		return (3);
	else if (str[0] == '>' && str[1] == 0)
		return (4);
	else if (str[0] == '>' && str[1] == '>' && str[2] == 0)
		return (5);
	else
		return (0);
}

void	free_lex_node(t_lexer *node)
{
	free(node->str);
	free(node);
}

void	lex_clean(t_lexer **root)
{
	t_lexer	*curr;
	t_lexer	*prev;
	t_lexer	*next;

	curr = *root;
	prev = NULL;
	while (curr)
	{
		next = curr->next;
		if (curr->str && curr->str[0] == '$' && !curr->is_expended
			&& !curr->was_quoted)
		{
			if (prev)
				prev->next = curr->next;
			else
				*root = curr->next;
			free_lex_node(curr);
		}
		else
		{
			prev = curr;
		}
		curr = next;
	}
}
