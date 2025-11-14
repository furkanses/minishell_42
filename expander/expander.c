/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:12:55 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 09:24:24 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

int	is_var_char(int c)
{
	return (ft_isalnum(c) || c == '_');
}

void	expand_tilda(int *l, int *i, t_env *env, t_local local)
{
	char	*home;
	int		j;

	home = get_env_cont_for_type(env, "HOME", 4);
	if (home)
	{
		*local.is_expanded1 = 1;
		j = -1;
		while (home[++j])
			local.iter->str[(*l)++] = home[j];
	}
	(*i)++;
}

void	expand_dollar(t_local local, t_env *env, char *src, int *is_expanded)
{
	int		k;
	int		j;
	char	*str;
	int		n;

	k = 0;
	while (src[*local.p + k + 1] && (ft_isalnum(src[*local.p + k + 1])
			|| src[*local.p + k + 1] == '_'))
		k++;
	str = get_env_cont_for_type(env, src + *local.p + 1, k);
	if (str)
	{
		*is_expanded = 1;
		j = 0;
		while (str[j])
			local.dst[(*local.b)++] = str[j++];
	}
	else
	{
		n = -1;
		local.dst[(*local.b)++] = '$';
		while (++n < k)
			local.dst[(*local.b)++] = src[*local.p + 1 + n];
	}
	*local.p += (k + 1);
}

t_dollar	*init_dollar(char *cmd, t_garbage **garb)
{
	t_dollar	*dollar;
	size_t		n;

	dollar = malloc(sizeof(t_dollar));
	if (!g_collecter(garb, dollar, 1))
		return (NULL);
	dollar->cmd = cmd;
	n = ft_strlen(dollar->cmd);
	dollar->ret = malloc(n + 1);
	if (!g_collecter(garb, dollar->ret, 1))
		return (NULL);
	return (dollar);
}
