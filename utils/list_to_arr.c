/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_to_arr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:06:27 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 01:35:17 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

static int	env_count(t_env *env)
{
	int		n;
	t_env	*it;

	n = 0;
	it = env;
	while (it)
	{
		n++;
		it = it->next;
	}
	return (n);
}

static int	itoa_shlvl(char **dst, int lvl, t_garbage **garb)
{
	char	*tmp;
	size_t	len;

	tmp = ft_itoa(lvl);
	if (!tmp)
		return (0);
	len = ft_strlen(tmp) + 7 + 1;
	*dst = (char *)malloc(len);
	if (!*dst || !g_collecter(garb, *dst, 1))
		return (0);
	(*dst)[0] = '\0';
	ft_strlcat(*dst, "SHLVL=", len);
	ft_strlcat(*dst, tmp, len);
	free(tmp);
	return (1);
}

static int	build_kv_string(char **out, const char *t, const char *c,
		t_garbage **garb)
{
	size_t	tl;
	size_t	cl;
	size_t	cap;

	tl = ft_strlen(t);
	cl = ft_strlen(c);
	cap = tl + cl + 2;
	*out = (char *)malloc(cap);
	if (!*out || !g_collecter(garb, *out, 1))
		return (0);
	(*out)[0] = '\0';
	ft_strlcat(*out, t, cap);
	ft_strlcat(*out, "=", cap);
	ft_strlcat(*out, c, cap);
	return (1);
}

static int	make_kv(char **out, t_env *it, t_garbage **garb)
{
	const char	*t;
	const char	*c;

	if (it->type && ft_strcmp(it->type, "SHLVL") == 0)
		return (itoa_shlvl(out, ft_atoi(it->content) + 1, garb));
	if (it->type)
		t = it->type;
	else
		t = "";
	if (it->content)
		c = it->content;
	else
		c = "";
	return (build_kv_string(out, t, c, garb));
}

char	**list_to_arr(t_env *env, t_garbage **garb)
{
	int		n;
	int		i;
	char	**envp;
	t_env	*it;

	n = env_count(env);
	envp = (char **)malloc(sizeof(char *) * (n + 1));
	if (!envp || !g_collecter(garb, envp, 1))
		return (NULL);
	it = env;
	i = 0;
	while (i < n)
	{
		if (!make_kv(&envp[i], it, garb))
			return (NULL);
		it = it->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}
