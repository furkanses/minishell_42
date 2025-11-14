/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 19:05:35 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 01:31:49 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <readline/readline.h>
#include "../minishell.h"

void	set_parent_handlers(void)
{
	struct sigaction	sa;
	struct sigaction	ign;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = parent_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &ign, NULL);
}

void	parent_ignore_exec_signals(void)
{
	struct sigaction	ign;

	sigemptyset(&ign.sa_mask);
	ign.sa_flags = 0;
	ign.sa_handler = SIG_IGN;
	sigaction(SIGINT, &ign, NULL);
	sigaction(SIGQUIT, &ign, NULL);
}

void	child_restore_default_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = child_sigquit_handler;
	sigaction(SIGQUIT, &sa, NULL);
}
