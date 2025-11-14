/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:24:19 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/08 12:32:26 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	skip_escaped(const char *s, int *i, int *l)
{
	if (s[*i] == '\\' && s[*i + 1])
	{
		(*l)++;
		*i += 2;
		return (1);
	}
	return (0);
}

void	process_tilde_env_or_char(int *i, t_local *local, t_env *env,
		int old_token)
{
	local->old_token = old_token;
	if (try_tilde_or_env(local, i, &local->l, env))
		(*i)--;
	else
		local->l++;
}

int	skip_ws(const char *str, int i, int t)
{
	int	j;

	j = 0;
	if (t)
	{
		while (!(str[i] == ' ' || (str[i] <= '\r' && str[i] >= '\t')) && str[i])
		{
			i++;
			j++;
		}
	}
	while ((str[i] == ' ' || (str[i] <= '\r' && str[i] >= '\t')) && str[i])
	{
		i++;
		j++;
	}
	return (j);
}
