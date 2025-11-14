/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repair_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:22:31 by fses              #+#    #+#             */
/*   Updated: 2025/09/08 13:11:57 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

void	dq_env_expand(int *i, int *l, t_env *env, t_quote quote)
{
	int		k;
	char	*val;

	if (quote.s[*i] == '$' && (ft_isalnum(quote.s[*i + 1]) || quote.s[*i
				+ 1] == '_') && quote.old_token != 2)
	{
		k = 0;
		while (quote.s[*i + k + 1] && (ft_isalnum(quote.s[*i + k + 1])
				|| quote.s[*i + k + 1] == '_'))
			k++;
		val = get_env_cont_for_type(env, quote.s + *i + 1, k);
		if (val && ft_strlen(val) > 0)
			*l += (int)ft_strlen(val);
		else
			*l += k;
		*i += k;
	}
	else
	{
		(*l)++;
		(*i)++;
	}
}

void	tilda_len(char *str, int *l, int *i, t_env *env)
{
	(void)str;
	(void)i;
	*l += ft_strlen(get_env_cont_for_type(env, "HOME", 4));
}

void	envir_len(char *str, int *i, int *l, t_env *env)
{
	int		k;
	char	*env_value;

	k = 0;
	while (str[*i + k + 1] && (ft_isalnum(str[*i + k + 1]) || str[*i + k
				+ 1] == '_'))
		k++;
	env_value = get_env_cont_for_type(env, str + *i + 1, k);
	if (env_value && ft_strlen(env_value) > 0)
		*l += ft_strlen(env_value);
	else
		*l += k;
	*i += k;
}

static t_lexer	*expand_and_splice(t_local local, t_lexer **root, t_env *env,
		t_garbage **garb)
{
	t_lexer	*new_list;
	t_lexer	*tail;
	t_lexer	*p;

	new_list = lexer(local.iter->str, env, garb);
	p = new_list;
	while (p)
	{
		p->is_expended = 1;
		p = p->next;
	}
	tail = new_list;
	while (tail->next)
		tail = tail->next;
	if (local.prev)
		local.prev->next = new_list;
	else
		*root = new_list;
	tail->next = local.iter->next;
	return (tail->next);
}

void	repair_expand(t_lexer **root, t_env *env, t_garbage **garb)
{
	t_local	local;
	int		i;

	local.iter = *root;
	local.prev = NULL;
	i = 0;
	while (local.iter)
	{
		if (local.iter->is_expended && !local.iter->was_quoted)
			local.iter = expand_and_splice(local, root, env, garb);
		else
		{
			local.prev = local.iter;
			local.iter = local.iter->next;
		}
	}
	local.iter = *root;
	while (local.iter)
	{
		local.iter->i = i++;
		local.iter = local.iter->next;
	}
}
