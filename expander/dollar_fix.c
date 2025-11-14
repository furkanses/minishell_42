/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_fix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:11:29 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 10:05:21 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

static void	handle_quotes(char c, char *q, size_t *i, t_dollar *dollar)
{
	if (c == '\'' || c == '"')
	{
		if (*q == c)
		{
			if (dollar->cmd[*i - 1] == '$')
				dollar->ret[(*dollar->l)++] = '$';
			*q = 0;
		}
		else if (*q == 0)
			*q = c;
		dollar->ret[(*dollar->l)++] = dollar->cmd[(*i)++];
	}
}

static void	copy_dollar_var(size_t *l, size_t *i, size_t len, t_dollar *dollar)
{
	while (len--)
		dollar->ret[(*l)++] = dollar->cmd[(*i)++];
}

static void	handle_dollar(size_t *l, size_t *i, t_env *env, t_dollar *dollar)
{
	size_t	j;
	size_t	len;

	j = *i + 1;
	while (is_var_char(dollar->cmd[j]))
		j++;
	len = j - (*i + 1);
	if (len == 4 && get_env_cont_for_type(env, dollar->cmd + *i + 1, len))
		copy_dollar_var(l, i, len, dollar);
	else
		*i = j;
}

static void	append_trailing_dollar(char *ret, size_t *l, char *cmd, size_t i)
{
	if (i > 0 && cmd[i - 1] == '$')
		ret[(*l)++] = '$';
}

char	*dollar_fix(char *cmd, t_env *env, t_garbage **garb)
{
	size_t		i;
	size_t		l;
	char		q;
	t_dollar	*dollar;

	dollar = init_dollar(cmd, garb);
	if (!dollar)
		return (NULL);
	dollar->l = &l;
	i = 0;
	l = 0;
	q = 0;
	while (dollar->cmd[i])
	{
		if (dollar->cmd[i] == '\'' || dollar->cmd[i] == '"')
			handle_quotes(cmd[i], &q, &i, dollar);
		else if (cmd[i] == '$' && q != '\'')
			handle_dollar(&l, &i, env, dollar);
		else
			dollar->ret[l++] = dollar->cmd[i++];
	}
	append_trailing_dollar(dollar->ret, &l, dollar->cmd, i);
	dollar->ret[l] = '\0';
	return (dollar->ret);
}
