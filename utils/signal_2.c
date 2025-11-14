/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 02:03:58 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 02:12:43 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

static volatile int	g_lobal = 0;

int	global_access(int change, int data)
{
	if (change)
		g_lobal = data;
	return (g_lobal);
}

void	heredoc_interrupt_handler(int signum)
{
	(void)signum;
	g_lobal = 1;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	parent_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	child_sigquit_handler(int sig)
{
	pid_t	g_child_pid;

	(void)sig;
	g_child_pid = (pid_t)g_lobal;
	g_lobal = -1;
	if (g_child_pid && kill(g_child_pid, SIGTERM) == -1)
	{
		perror("kill");
	}
	if (!g_child_pid)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
