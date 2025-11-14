/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:49:33 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:24:13 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_file_execution(t_local *local, t_env *envir, t_garbage **garb)
{
	int		exit_code;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	exit_code = execute(local->ast_root, envir, garb, &redir);
	g_free_l(garb);
	return (exit_code);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	is_all_ws(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		if (!is_ws(str[i]))
			return (0);
	return (1);
}
