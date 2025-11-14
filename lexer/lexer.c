/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:08:26 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:10:58 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

static void	handle_special_chars(t_local *local, t_env *env, int *is_expended)
{
	if (local->cmd[local->j] == '~' && (local->j == 0
			|| is_ws(local->cmd[local->j - 1])) && (local->cmd[local->j
				+ 1] == '/' || is_ws(local->cmd[local->j + 1])
			|| local->cmd[local->j + 1] == 0) && !local->q
		&& local->old_token != 2)
		expand_tilda(&local->l, &local->j, env, *local);
	else if (local->cmd[local->j] == '$' && (ft_isalnum(local->cmd[local->j
					+ 1]) || local->cmd[local->j + 1] == '_') && !local->q
		&& local->old_token != 2)
	{
		local->b = &local->l;
		local->p = &local->j;
		local->dst = local->iter->str;
		expand_dollar(*local, env, local->cmd, is_expended);
	}
	else
		local->iter->str[local->l++] = local->cmd[local->j++];
}

static int	lex_cpy(t_local local, t_env *env, int *was_quoted_ptr,
		int *is_expended)
{
	local.is_expanded1 = is_expended;
	assign_lex_cpy(&local, was_quoted_ptr, is_expended, local.j);
	while (!is_ws(local.cmd[local.j]) && local.cmd[local.j])
	{
		if (handle_escape_char(local.iter->str, local.cmd, &local.j, &local.l)
			&& !local.q)
			continue ;
		if (handle_quote_block(&local.j, &local, env, was_quoted_ptr))
			continue ;
		handle_special_chars(&local, env, is_expended);
	}
	local.iter->str[local.l] = '\0';
	return (local.j - local.start_i);
}

int	handle_next_lexer_node(char *cmd, int j, t_lexer **iter, t_garbage **garb)
{
	if (cmd[j])
	{
		(*iter)->next = malloc(sizeof(t_lexer));
		if (!(*iter)->next)
			return (0);
		g_collecter(garb, (*iter)->next, 1);
		*iter = (*iter)->next;
		(*iter)->str = NULL;
		(*iter)->token = 0;
		(*iter)->i = 0;
		(*iter)->is_expended = 0;
		(*iter)->was_quoted = 0;
		(*iter)->next = NULL;
		return (1);
	}
	else
	{
		(*iter)->next = NULL;
		return (1);
	}
}

t_lexer	*init_lexer(char *cmd, t_local *local, t_garbage **garb)
{
	local->j = 0;
	local->i = 0;
	local->old_token = 0;
	local->root = malloc(sizeof(t_lexer));
	if (!local->root)
		return (NULL);
	g_collecter(garb, local->root, 1);
	local->root->str = NULL;
	local->root->token = 0;
	local->root->i = 0;
	local->root->is_expended = 0;
	local->root->was_quoted = 0;
	local->root->next = NULL;
	local->iter = local->root;
	local->j += skip_ws(cmd, local->j, 0);
	return (local->root);
}

t_lexer	*lexer(char *cmd, t_env *envir, t_garbage **garb)
{
	t_local	local;

	local.cmd = cmd;
	if (!init_lexer(cmd, &local, garb))
		return (NULL);
	while (cmd[local.j])
	{
		local.token_len = get_len(cmd, local.j - 1, envir, local.old_token);
		local.iter->str = malloc(local.token_len + 1);
		g_collecter(garb, local.iter->str, 1);
		local.consumed_len = lex_cpy(local, envir, &local.was_quoted,
				&local.is_expended);
		if (!local.was_quoted)
			local.iter->token = token(local.iter->str);
		local.old_token = local.iter->token;
		local.iter->was_quoted = local.was_quoted;
		local.iter->is_expended = local.is_expended;
		local.iter->i = local.i++;
		local.j += local.consumed_len;
		local.j += skip_ws(cmd, local.j, 0);
		if (!handle_next_lexer_node(cmd, local.j, &local.iter, garb))
			return (NULL);
	}
	return (local.root);
}
