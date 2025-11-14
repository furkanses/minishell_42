/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_stufs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:20:46 by fses              #+#    #+#             */
/*   Updated: 2025/09/08 12:16:31 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

int	quote_count(char *str)
{
	int		count;
	char	quote;

	count = 0;
	quote = 0;
	while (*str)
	{
		if ((*str == '\'' || *str == '"'))
		{
			if (quote == 0)
			{
				quote = *str;
			}
			else if (quote == *str)
			{
				count++;
				quote = 0;
			}
		}
		str++;
	}
	return (count);
}

static void	process_quoted_content(t_local local, int *l, t_env *env,
		int *is_expended)
{
	local.dst = local.iter->str;
	local.b = l;
	local.p = local.c;
	while (local.cmd[*local.p] && local.cmd[*local.p] != local.q)
	{
		if (local.q == '"' && local.cmd[*local.p] == '\\'
			&& (local.cmd[*local.p + 1] == '"'
				|| local.cmd[*local.p + 1] == '\\'
				|| local.cmd[*local.p + 1] == '$'))
		{
			(*local.p)++;
			local.dst[(*local.b)++] = local.cmd[(*local.p)++];
		}
		else if (local.q == '"' && local.cmd[*local.p] == '$'
			&& (ft_isalnum(local.cmd[*local.p + 1]) || local.cmd[*local.p
					+ 1] == '_') && local.old_token != 2)
		{
			expand_dollar(local, env, local.cmd, is_expended);
		}
		else
		{
			local.dst[(*local.b)++] = local.cmd[(*local.p)++];
		}
	}
}

int	handle_quote_block(int *i, t_local *local, t_env *env, int *was_quoted_ptr)
{
	local->c = i;
	if (!local->q && (local->cmd[*i] == '"' || local->cmd[*i] == '\''))
	{
		if (*was_quoted_ptr == 0)
		{
			if (local->cmd[*i] == '"')
				*was_quoted_ptr = 6;
			else
				*was_quoted_ptr = 7;
		}
		local->q = local->cmd[*i];
		(*i)++;
		process_quoted_content(*local, &local->l, env, local->is_expanded1);
		if (local->cmd[*i] == local->q)
			(*i)++;
		local->q = 0;
		return (1);
	}
	return (0);
}
