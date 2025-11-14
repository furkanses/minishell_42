/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_permissions.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fses <fses@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by fses              #+#    #+#             */
/*   Updated: 2025/01/27 00:00:00 by fses             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int	check_read_permission(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	close(fd);
	return (1);
}

int	check_write_permission(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	close(fd);
	return (1);
}

int	check_append_permission(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)filename, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (0);
	}
	close(fd);
	return (1);
}

int	file_exists(const char *filename)
{
	struct stat	st;

	if (stat(filename, &st) == 0)
		return (1);
	return (0);
}

int	check_redirect_permissions(t_ast *node)
{
	if (!file_exists(node->filename))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->filename, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	if (!check_read_permission(node->filename))
		return (0);
	else if ((node->type == NODE_REDIR_OUT || node->type == NODE_REDIR_APPEND)
		&& node->filename)
	{
		if (node->type == NODE_REDIR_OUT
			&& !check_write_permission(node->filename))
			return (0);
		else if (node->type == NODE_REDIR_APPEND
			&& !check_append_permission(node->filename))
			return (0);
	}
	return (1);
}
