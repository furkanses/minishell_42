/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdinc <mdinc@student.42kocaeli.com.tr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 15:47:16 by mdinc             #+#    #+#             */
/*   Updated: 2025/09/15 09:24:04 by mdinc            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../minishell.h"
#include <stdlib.h>

void	handle_syntax_error(t_local *local)
{
	if (local->flag == 0)
	{
		ft_putstr_fd("-minishell: syntax error", 2);
		ft_putstr_fd("near unexpected token `newline'\n", 2);
	}
	else if (local->flag == 1)
		ft_putstr_fd("-minishell: syntax error near unexpected token `|'\n", 2);
}

int	process_input(char *s, t_env *envir, char **env, t_garbage **garb)
{
	t_local	local;
	int		exit_code;

	(void)env;
	exit_code = initialize_and_lex(s, envir, garb, &local);
	if (exit_code != 0)
		return (exit_code);
	repair_expand(&local.lexer_root, envir, garb);
	if (syntax_check(local.lexer_root, &local.flag) == 1)
	{
		exit_code = handle_valid_syntax(&local, envir, garb);
		if (exit_code >= 256)
			return (exit_code);
	}
	else
	{
		handle_syntax_error(&local);
		return (2);
	}
	g_free_l(garb);
	return (exit_code);
}

int	handle_empty_input(t_local *local)
{
	if (is_all_ws(local->s))
	{
		g_free_l(&local->garb);
		return (1);
	}
	if (*local->s == '\0')
	{
		free(local->s);
		return (1);
	}
	return (0);
}

int	handle_valid_syntax(t_local *local, t_env *envir, t_garbage **garb)
{
	local->ast_root = parse_tokens(local->lexer_root, garb);
	if (!local->ast_root)
		return (1);
	if (ft_strncmp("exit", local->lexer_root->str, 4) == 0)
		return (handle_exit_command(local, envir, garb));
	else if (file_check(local->lexer_root) == 1)
		return (handle_file_execution(local, envir, garb));
	else
		new_process(local->lexer_root);
	return (0);
}

int	handle_exit_command(t_local *local, t_env *envir, t_garbage **garb)
{
	int		exit_code;
	t_redir	redir;

	redir.delimiter = 1;
	redir.redir_in = 1;
	ft_putstr_fd("exit\n", 1);
	exit_code = execute(local->ast_root, envir, garb, &redir);
	if (exit_code != 257)
	{
		g_free(garb);
		return (256 + exit_code);
	}
	return (exit_code);
}
