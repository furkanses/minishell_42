/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   question.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 18:19:29 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:28:35 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include "stdlib.h"

static int	count_size(unsigned int n)
{
	int	i;

	i = 1;
	while (n >= 10)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int num)
{
	unsigned int	n;
	int				count;
	char			*dst;

	n = (unsigned int)num;
	count = count_size(n);
	dst = (char *)malloc((size_t)count + 1);
	if (!dst)
		return (NULL);
	dst[count] = '\0';
	while (count--)
	{
		dst[count] = (char)('0' + (n % 10));
		n /= 10;
	}
	return (dst);
}

static size_t	expend_question_len(int exit_code, const char *s)
{
	size_t	hits;
	int		digits;
	int		i;

	hits = 0;
	i = -1;
	while (s[++i])
		if (s[i] == '$' && s[i + 1] == '?')
			hits++;
	digits = count_size((unsigned int)exit_code);
	return (ft_strlen(s) + hits * (size_t)(digits - 2));
}

static char	*expand_with_exit_code(const char *s, const char *code,
		int exit_code, t_garbage **garb)
{
	size_t	i;
	size_t	l;
	size_t	j;
	char	*ret;

	ret = (char *)malloc(expend_question_len(exit_code, (char *)s) + 1);
	if (!g_collecter(garb, ret, 1))
		return (NULL);
	i = 0;
	l = 0;
	while (s[i])
	{
		if (s[i] == '$' && s[i + 1] == '?')
		{
			j = 0;
			while (code[j])
				ret[l++] = code[j++];
			i += 2;
		}
		else
			ret[l++] = s[i++];
	}
	ret[l] = '\0';
	return (ret);
}

char	*expend_question(int exit_code, char *s, t_garbage **garb)
{
	char	*code;
	char	*ret;

	code = ft_itoa(exit_code);
	if (!code)
		return (NULL);
	ret = expand_with_exit_code(s, code, exit_code, garb);
	free(code);
	free(s);
	return (ret);
}
