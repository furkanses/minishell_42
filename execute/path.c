/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:54:08 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 10:01:04 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

size_t	process_path_normalization(const char *path, char *out,
		size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < n)
	{
		if (path[i] == ':')
		{
			if (i == 0 || path[i - 1] == ':')
				out[j++] = '.';
			out[j++] = ':';
		}
		else
			out[j++] = path[i];
		i++;
	}
	return (j);
}

static size_t	join_len(const char *dir, const char *cmd)
{
	size_t	dlen;
	size_t	clen;
	int		need_slash;

	dlen = ft_strlen(dir);
	clen = ft_strlen(cmd);
	need_slash = 1;
	if (dlen > 0 && dir[dlen - 1] == '/')
		need_slash = 0;
	return (dlen + (size_t)need_slash + clen + 1);
}

static void	join_path(char *dst, const char *dir, const char *cmd)
{
	size_t	dlen;

	dst[0] = '\0';
	ft_strcpy(dst, dir);
	dlen = ft_strlen(dir);
	if (dlen == 0 || dst[dlen - 1] != '/')
		ft_strcat(dst, "/");
	ft_strcat(dst, cmd);
}

char	**split_dirs(char *norm, t_garbage **garb)
{
	char	**dirs;
	size_t	i;

	dirs = ft_split(norm, ':');
	if (!dirs)
		return (NULL);
	g_collecter(garb, dirs, 1);
	i = 0;
	while (dirs[i])
	{
		g_collecter(garb, dirs[i], 1);
		i++;
	}
	return (dirs);
}

char	*search_in_dirs(char **dirs, char *cmd, t_garbage **garb)
{
	size_t	i;
	size_t	flen;
	char	*full;

	i = 0;
	while (dirs && dirs[i])
	{
		flen = join_len(dirs[i], cmd);
		full = (char *)malloc(flen);
		if (!full)
			return (NULL);
		g_collecter(garb, full, 1);
		join_path(full, dirs[i], cmd);
		if (access(full, X_OK) == 0)
			return (full);
		i++;
	}
	return (NULL);
}
