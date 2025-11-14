/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_len.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 17:58:11 by fses              #+#    #+#             */
/*   Updated: 2025/09/08 12:33:47 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

static void	handle_squote(const char *s, int *i, int *l, int *qcnt)
{
	int	q;

	q = '\'';
	(*l)++;
	(*i)++;
	while (s[*i] && s[*i] != q)
	{
		(*l)++;
		(*i)++;
	}
	if (s[*i] == q)
	{
		(*l)++;
		(*i)++;
		(*qcnt)++;
	}
}

static void	handle_dquote(int *l, int *qcnt, t_env *env, t_quote quote)
{
	int	q;

	q = '"';
	(*l)++;
	(*quote.i)++;
	while (quote.s[*quote.i] && quote.s[*quote.i] != q)
	{
		if (quote.s[*quote.i] == '\\' && quote.s[*quote.i + 1]
			&& (quote.s[*quote.i + 1] == '"' || quote.s[*quote.i + 1] == '\\'
				|| quote.s[*quote.i + 1] == '$'))
		{
			(*l)++;
			*quote.i += 2;
		}
		else
			dq_env_expand(quote.i, l, env, quote);
	}
	if (quote.s[*quote.i] == q)
	{
		(*l)++;
		(*quote.i)++;
		(*qcnt)++;
	}
}

int	try_tilde_or_env(t_local *local, int *i, int *l, t_env *env)
{
	if (local->st[*i] == '~' && (local->st[*i + 1] == '/' || is_ws(local->st[*i
					+ 1])) && (*i == 0 || is_ws(local->st[*i - 1])) && local->q
		&& local->old_token != 2)
	{
		tilda_len((char *)local->st, l, i, env);
		return (1);
	}
	if (local->st[*i] == '$' && !is_ws(local->st[*i + 1]) && local->q != '\''
		&& (ft_isalnum(local->st[*i + 1]) || local->st[*i + 1] == '_')
		&& local->old_token != 2)
	{
		envir_len((char *)local->st, i, l, env);
		return (1);
	}
	return (0);
}

static int	process_escape_or_quotes(t_quote quote, int *i, t_local *local,
		t_env *env)
{
	if (skip_escaped(quote.st, i, &local->l))
	{
		(*i)--;
		return (1);
	}
	if (!local->q && quote.st[*i] == '\'')
	{
		handle_squote(quote.st, i, &local->l, &local->quote_count);
		(*i)--;
		return (1);
	}
	if (!local->q && quote.st[*i] == '"')
	{
		quote.i = i;
		quote.old_token = quote.old_token;
		quote.s = quote.st;
		handle_dquote(&local->l, &local->quote_count, env, quote);
		(*i)--;
		return (1);
	}
	return (0);
}

int	get_len(char *str, int i, t_env *env, int old_token)
{
	t_local	local;
	t_quote	quote;

	local.l = 0;
	local.q = 0;
	local.quote_count = 0;
	if (!str)
		return (0);
	while (str[i + 1] && !is_ws(str[i + 1]))
	{
		i++;
		quote.st = str;
		quote.old_token = old_token;
		if (process_escape_or_quotes(quote, &i, &local, env))
			continue ;
		local.st = str;
		process_tilde_env_or_char(&i, &local, env, old_token);
	}
	return (local.l - local.quote_count * 2);
}
