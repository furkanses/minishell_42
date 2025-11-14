/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_space.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fses <fses@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:18:18 by fses              #+#    #+#             */
/*   Updated: 2025/08/28 18:18:47 by fses             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

int	more_space_len(char *cmd)
{
	int	i;
	int	l;
	int	q;

	i = -1;
	l = 0;
	q = 0;
	if (!cmd)
		return (0);
	while (cmd[++i])
	{
		if (!q && (cmd[i] == '\'' || cmd[i] == '"'))
			q = cmd[i];
		else if (q && cmd[i] == q)
			q = 0;
		if ((cmd[i] == '<' || cmd[i] == '|' || cmd[i] == '>') && !q)
		{
			l++;
			while ((cmd[i] == '<' || cmd[i] == '>' || cmd[i] == '|') && cmd[i])
				i++;
			l++;
			i--;
		}
	}
	return (i + 1 + 2 * l);
}

char	*init_more_space(char *cmd, t_local *local, t_garbage **garb)
{
	local->i = -1;
	local->l = 0;
	local->q = 0;
	local->str = malloc(more_space_len(cmd) + 1);
	g_collecter(garb, local->str, 1);
	if (!local->str)
		return (NULL);
	return (local->str);
}

char	*more_space(char *cmd, t_garbage **garb)
{
	t_local	local;

	if (!init_more_space(cmd, &local, garb))
		return (NULL);
	while (cmd[++local.i])
	{
		if (!local.q && (cmd[local.i] == '\'' || cmd[local.i] == '"'))
			local.q = cmd[local.i];
		else if (local.q && cmd[local.i] == local.q)
			local.q = 0;
		if ((cmd[local.i] == '<' || cmd[local.i] == '>' || cmd[local.i] == '|')
			&& !local.q)
		{
			local.str[local.l++] = ' ';
			while ((cmd[local.i] == '<' || cmd[local.i] == '>'
					|| cmd[local.i] == '|') && cmd[local.i])
				local.str[local.l++] = cmd[local.i++];
			local.str[local.l++] = ' ';
			local.i--;
		}
		else
			local.str[local.l++] = cmd[local.i];
	}
	local.str[local.l] = '\0';
	return (local.str);
}
