/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:21:26 by fses              #+#    #+#             */
/*   Updated: 2025/09/15 00:14:12 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdio.h>
#include <unistd.h>

static int	update_env(char *typ, char *new_cont, t_env **env, t_garbage **garb)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strncmp(tmp->type, typ, ft_strlen(typ)) == 0)
		{
			tmp->content = ft_strdup(new_cont);
			g_collecter_env(garb, tmp->content);
			if (!tmp->content)
				return (1);
			return (0);
		}
		tmp = tmp->next;
	}
	return (0);
}

static int	update_oldpwd_env(t_env **env, t_garbage **garb)
{
	char	*cwd;
	char	*oldpwd_val;
	char	*cwd_copy;

	cwd = get_env_cont_for_type(*env, "PWD", 3);
	if (!cwd)
	{
		perror("cd: PWD not set");
		return (1);
	}
	oldpwd_val = get_env_cont_for_type(*env, "OLDPWD", 6);
	if (oldpwd_val)
	{
		oldpwd_val = ft_strdup(oldpwd_val);
		g_collecter(garb, oldpwd_val, 0);
	}
	cwd_copy = ft_strdup(cwd);
	if (!g_collecter(garb, cwd_copy, 0))
		return (1);
	if (update_env("OLDPWD", cwd_copy, env, garb))
		return (1);
	return (0);
}

static int	determine_target_directory(char **cmd, t_env **env, char **target)
{
	if (!cmd[1])
	{
		*target = get_env_cont_for_type(*env, "HOME", 4);
		if (!*target)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
		*target = cmd[1];
	return (0);
}

static int	setup_cd_environment(char **cmd, t_env **env, t_garbage **garb,
		char **target)
{
	if (update_oldpwd_env(env, garb))
		return (1);
	if (determine_target_directory(cmd, env, target))
		return (1);
	return (0);
}

int	builtin_cd(char **cmd, t_env **env, t_garbage **garb)
{
	char	*target;
	char	*new_pwd;

	usleep(100);
	if (setup_cd_environment(cmd, env, garb, &target))
		return (1);
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		update_env("PWD", new_pwd, env, garb);
		g_collecter(garb, new_pwd, 0);
	}
	return (0);
}
