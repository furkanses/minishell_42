/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 18:43:22 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:48:32 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_printf/ft_printf.h"
#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

static void	handle_child_process1(char **args, t_env *env, t_garbage **garb,
		t_child *child)
{
	child_restore_default_signals();
	if (child->cat_flag)
		global_access(1, getpid());
	execve(child->cmd_path, args, list_to_arr(env, garb));
	perror("execve");
	exit(1);
}

static int	handle_parent_process1(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	set_parent_handlers();
	return (ret_status(status));
}

static int	execute_child_process(char *cmd_path, char **args, t_env *env,
		t_garbage **garb)
{
	pid_t	pid;
	t_child	child;

	child.cat_flag = 0;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	child.cmd_path = cmd_path;
	parent_ignore_exec_signals();
	if (ft_strncmp(args[0], "./", 1) != 0)
		child.cat_flag++;
	if (pid == 0)
		handle_child_process1(args, env, garb, &child);
	else
		return (handle_parent_process1(pid));
	return (1);
}

int	exec_cmd_node(t_ast *node, t_env *env, t_garbage **garb)
{
	char		*cmd;
	int			has_slash;
	char		*cmd_path;
	const char	*path;
	int			validation_result;

	if (!node || !node->args)
		return (0);
	if (is_builtin(node->args[0]))
		return (exec_builtin(node->args, env, garb));
	cmd = node->args[0];
	has_slash = (ft_strchr(cmd, '/') != NULL);
	cmd_path = find_command_path(node->args[0], garb, env);
	if (has_slash)
		path = cmd;
	else
		path = cmd_path;
	validation_result = validate_command_path(path, cmd, cmd_path);
	if (validation_result != 0)
		return (validation_result);
	return (execute_child_process(cmd_path, node->args, env, garb));
}
