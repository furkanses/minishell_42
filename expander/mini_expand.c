/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_expand.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:16:49 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:10:47 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

void	expend_dollar_else(t_local *local, t_env *env, char *src)
{
	local->val = get_env_cont_for_type(env, src + local->i + 1, local->k);
	if (local->val)
	{
		local->j = 0;
		while (local->val[local->j])
			local->out[local->o++] = local->val[local->j++];
	}
	local->i += 1 + local->k;
}

static int	handle_dollar(t_env *env, char *src, t_local *local)
{
	local->k = 0;
	while (src[local->i + 1 + local->k]
		&& is_var_char((unsigned char)src[local->i + 1 + local->k]))
		local->k++;
	if (local->k == 0)
	{
		counter_expand(&local->total, &local->i);
		return (1);
	}
	local->val = get_env_cont_for_type(env, src + local->i + 1, local->k);
	if (local->val)
		local->total += (int)ft_strlen(local->val);
	local->i += 1 + local->k;
	return (1);
}

int	mini_expend_dollar_len(t_env *env, char *src)
{
	t_local	local;

	local.i = 0;
	local.total = 0;
	while (src[local.i])
	{
		if (src[local.i] == '$')
			handle_dollar(env, src, &local);
		else
			counter_expand(&local.total, &local.i);
	}
	return (local.total);
}

void	expend_assign_value(t_local *local)
{
	local->i = 0;
	local->o = 0;
}

char	*mini_expend_dollar(t_env *env, char *src, t_garbage **garb)
{
	t_local	local;

	(void)garb;
	expend_assign_value(&local);
	local.out_len = mini_expend_dollar_len(env, src);
	local.out = (char *)malloc((size_t)local.out_len + 1);
	while (src[local.i])
	{
		if (src[local.i] == '$')
		{
			local.k = 0;
			while (src[local.i + 1 + local.k]
				&& is_var_char((unsigned char)src[local.i + 1 + local.k]))
				local.k++;
			if (local.k == 0)
				local.out[local.o++] = src[local.i++];
			else
				expend_dollar_else(&local, env, src);
		}
		else
			local.out[local.o++] = src[local.i++];
	}
	local.out[local.o] = '\0';
	return (local.out);
}
