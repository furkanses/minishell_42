/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 12:42:36 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/08 16:03:59 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"

int	is_ws(char c)
{
	return (c == ' ' || (c <= '\r' && c >= '\t'));
}

char	*get_env_cont_for_type(t_env *env, const char *type, int n)
{
	t_env	*iter;

	iter = env;
	while (iter)
	{
		if (ft_strncmp(iter->type, type, n) == 0 && iter->type[n] == 0)
			return (iter->content);
		iter = iter->next;
	}
	return (NULL);
}

char	*get_env_type_for_cont(t_env *env, char *cont, int n)
{
	t_env	*iter;

	iter = env;
	while (iter)
	{
		if (ft_strncmp(iter->content, cont, n) == 0 && iter->content[n] == 0)
			return (iter->type);
		iter = iter->next;
	}
	return (NULL);
}

int	quote_check(const char *cmd)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (cmd[i])
	{
		if (cmd[i] == '\\' && quote != '\'')
		{
			i++;
			if (cmd[i])
				i++;
			continue ;
		}
		else if (!quote && (cmd[i] == '\'' || cmd[i] == '"'))
			quote = cmd[i];
		else if (cmd[i] == quote)
			quote = 0;
		i++;
	}
	return (quote == 0);
}

int	initialize_and_lex(char *s, t_env *envir, t_garbage **garb,
		t_local *local)
{
	local->flag = 0;
	local->lexer_root = lexer(s, envir, garb);
	if (!local->lexer_root)
		return (1);
	return (0);
}
